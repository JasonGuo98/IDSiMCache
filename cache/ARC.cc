#include "ARC.hh"

void myfree2(void *val)
{
    assert(val);
    free(val);
}

int64_t ARCCache::reset(int64_t new_capacity = 0)
{
    obj_table.clear();

    if (_t1 != nullptr)
    {
        list_destroy(_t1);
    }
    if (_b1 != nullptr)
        list_destroy(_b1);
    if (_t2 != nullptr)
        list_destroy(_t2);
    if (_b2 != nullptr)
        list_destroy(_b2);
    _t1 = list_new();
    _b1 = list_new();
    _t2 = list_new();
    _b2 = list_new();
    _t1->free = myfree2;
    _b1->free = myfree2;
    _t2->free = myfree2;
    _b2->free = myfree2;
    t1_p = 0;
    current_size = 0;
    current_access_count = 0;
    if (new_capacity != 0)
    {
        cache_capacity = new_capacity;
    }
    return 0;
}

int64_t ARCCache::get(int64_t key, void *p)
{
    if (cache_capacity == 0)
        return -1;

    auto iter = obj_table.find(key); //通过key-缓存对象的哈希表找到缓存对象
    if (iter == obj_table.end())     //没找到，返回-1
        return -1;

    oneCacheLine_t *cl = (oneCacheLine_t *)iter->second->val;

    if (cl->list == _t1 || cl->list == _t2) // case1，在lru1或lru2链表中找到了该缓存对象
    {
        //将缓存对象移动到lru2链表的首部
        move_to(iter->second, _t2);

        // lru1和lru2的总长度并没有改变，ghost_lru1和ghost_lru2的总长度也没有改变

        assert_c();
        return cl->value;
    }
    else if (cl->list == _b1) // case2，在ghost_lru1链表中找到了该缓存对象
    {
        //若ghost_lru1的长度大于等于ghost_lru2的长度，则使lru1的预期长度p加1，否则使p加(ghost_lru2的长度/ghost_lru1的长度)
        //原论文对这个策略的描述： Thus, smaller the size of B1 , the larger the increment.(B1就是这里的ghost_lru1)
        //(可能原论文中还有我没注意到的地方更具体地描述了这个策略是如何想出来的，也可能就是构造来的，总之最后实验效果是好的)
        auto t = _b1->len >= _b2->len ? 1 : _b2->len / (double)_b1->len;
        t1_p = min(t1_p + t, (double)cache_capacity); //预期长度不能超过缓存空间大小

        //由于存在将缓存对象从ghost链表移动到lru链表的操作，因此lru1和lru2链表的总长度会增加
        //而又需要维持lru1+lru2<=capability，因此需要将lru1或lru2中的尾部的缓存对象移动到ghost_lru1或ghost_lru2中
        replace(false);

        //将当前找到的缓存对象移动到lru2的首部
        move_to(iter->second, _t2);

        //从ghost_lru1向lru2中添加了缓存对象，又从lru1或lru2向ghost1或ghost2中删除了缓存对象
        //因此最后lru1和lru2以及ghost_lru1和ghost_lru2的总长度均没有改变

        assert_c();
        return cl->value;
    }
    else if (cl->list == _b2) // case3，在ghost_lru2中找到了该缓存对象
    {
        //若ghost_lru2的长度大于等于ghost_lru1的长度，则使lru1的预期长度p减1，否则使p减(ghost_lru1的长度/ghost_lru2的长度)
        //原论文对这个策略的描述： Thus, smaller the size of B2 , the larger the decrement.(B1就是这里的ghost_lru1)
        //(可能原论文中还有我没注意到的地方更具体地描述了这个策略是如何想出来的，也可能就是构造来的，总之最后实验效果是好的)
        auto t = _b2->len >= _b1->len ? 1 : _b1->len / (double)_b2->len;
        t1_p = max(t1_p - t, 0.0); //预期长度不能小于0

        replace(true);              //将lru1或lru2中的尾部的缓存对象移动到ghost_lru1或ghost_lru2中
        move_to(iter->second, _t2); //将当前找到的缓存对象移动到lru2的首部

        //从ghost_lru2向lru2中添加了缓存对象，又从lru1或lru2向ghost1或ghost2中删除了缓存对象
        //因此最后lru1和lru2以及ghost_lru1和ghost_lru2的总长度均没有改变

        assert_c();
        return cl->value; //返回对象地址
    }
    return 0;
}

int64_t ARCCache::set(int64_t key, int64_t value = 0)
{
    if (cache_capacity == 0)
        return 0;
    auto it = obj_table.find(key); //查找缓存列表中是否已经存在当前缓存对象
    if (it == obj_table.end())     //没找到，则插入
    {
        // case4，向lru1中插入新缓存对象，并做相应调整
        assert(_t1->len + _b1->len <= cache_capacity);
        if (_t1->len + _b1->len == cache_capacity) // case4.1，若lru1和ghost_lru1的总长度等于缓存空间大小
        {                                          //则需要维持lru1和ghost_lru1的总长度不大于缓存空间大小

            //若lru1的长度小于缓存空间大小，则说明ghost_lru1不为空，删除ghost_lru1的末尾元素
            if (_t1->len < cache_capacity)
            {
                list_node_t *del_item = list_rpop(_b1);
                oneCacheLine_t *del_cl = (oneCacheLine_t *)del_item->val;
                obj_table.erase(del_cl->key);
                delete (del_cl);
                delete (del_item);

                //由于要将新的缓存对象添加到lru1链表，因此lru1和lru2链表的总长度会增加
                //而又需要维持lru1+lru2<=capability，因此需要将lru1或lru2中的尾部的缓存对象移动到ghost_lru1或ghost_lru2中
                replace(false);
                //最后lru1和lru2总长度不变，ghost_lru1和ghost_lru2的总长度增加了1
            }
            else // lru1的长度等于缓存空间大小，说明ghost_lru1为空，则删除lru1末尾的元素
            {
                list_node_t *del_item = list_rpop(_t1);
                oneCacheLine_t *del_cl = (oneCacheLine_t *)del_item->val;
                obj_table.erase(del_cl->key);
                delete (del_cl);
                delete (del_item);
                current_size--;
                //后面要往lru1中添加新的缓存对象，这里又删除了lru1末尾的缓存对象，长度不变，因此这里不需要再做调整
            }
        }
        else // case 4.2，lru1和ghost_lru1的总长度小于缓存空间大小，则继续判断
        {
            assert(_t1->len + _b1->len < cache_capacity);
            //计算lru1、lru2、ghost_lru1、ghost_lru2的总长度
            auto size = _t1->len + _t2->len + _b1->len + _b2->len;
            if (size >= cache_capacity) //若总长度大于等于缓存空间大小，说明lru1和lru2总长度一定为缓存空间大小(根据算法的步骤可知，在lru1和lru2的总长度达到缓存空间大小之前，一定不会往ghost_lru1和ghost_lru2中添加元素)
            {
                if (size == cache_capacity * 2) //若总长度正好等于缓存空间大小的两倍，即lru1+ghost_lru1的长度<缓存空间大小,lru2+ghost_lru2的长度>缓存空间大小
                {                               //则需要维持总长度不大于2倍的缓存空间大小，因此删除ghost_lru2末尾的元素
                    list_node_t *del_item = list_rpop(_b2);
                    oneCacheLine_t *del_cl = (oneCacheLine_t *)del_item->val;
                    obj_table.erase(del_cl->key);
                    delete (del_cl);
                    delete (del_item);
                }
                //往lru1中添加了新的缓存对象，lru1和lru2的总长度增加，因此需要将lru1或lru2中的尾部的缓存对象移动到ghost_lru1或ghost_lru2中
                replace(false);
            }
        }
        //新建缓存对象，插入
        oneCacheLine_t *cl = new oneCacheLine_t{key, value, 0, current_access_count, _t1};
        list_node_t *new_node = list_node_new(cl);
        list_lpush(_t1, new_node); // lru1首部插入新缓存对象
        obj_table[key] = new_node;
        assert_c();
    }
    else //缓存中已经存在当前缓存对象，则将其值和使用频率更新
    {
        // 与 get 操作基本一致，除了需要更新缓存的值
        list_node_t *node = it->second;
        oneCacheLine_t *cl = (oneCacheLine_t *)node->val;
        cl->value = value;
        get(key, nullptr);
    }
    return 0;
}

int64_t ARCCache::next()
{
    current_access_count++;
    return current_access_count;
}

void ARCCache::move_to(list_node_t *node, list_t *list)
{
    oneCacheLine_t *cl = (oneCacheLine_t *)node->val;
    list_remove(cl->list, node);
    cl->list = list;
    list_lpush(list, node);
}

void ARCCache::replace(bool in_b2) //将lru1或lru2中的尾部的缓存对象移动到ghost_lru1或ghost_lru2中
{
    //论文中描述的逻辑：
    // 1、若lru1的长度大于其期望长度p，则将lru1末尾元素移动到ghost_lru1中
    // 2、若lru1的长度小于其期望长度p，则将lru2末尾元素移动到ghost_lru2中
    // 3、若lru1的长度等于其期望长度p且miss的缓存对象在ghost_lru1中，则将lru2末尾元素移动到ghost_lru2中
    //(即若lru1的长度等于其期望长度p且miss的缓存对象在ghost_lru2中，则将lru1末尾元素移动到ghost_lru1中)
    //在lru1的长度等于期望长度p时，不论miss的缓存对象在ghost_lru1中还是ghost_lru2中，选择删除lru1或lru2中的对象应该都是可以的
    //这里依照原文描述的逻辑实现，原文中对这个(第3个)条件判断的描述如下：
    // The last replacement decision is somewhat arbitrary, and can be made differently if desired.
    if (_t1->len != 0 &&
        ((_t1->len > t1_p) || (in_b2 && _t1->len == t1_p)))
    { //满足if所述条件，则将lru1中的缓存对象移动到ghost_lru1中

        oneCacheLine_t *cl = (oneCacheLine_t *)_t1->tail->val;
        cl->value = -1; //删除缓存对象的缓存
        assert(cl->list == _t1);
        move_to(_t1->tail, _b1); //将lru1中尾部的缓存对象标记加入ghost_lru1链表中
    }
    else //否则将lru2中的缓存对象移动到ghost_lru2中
    {
        oneCacheLine_t *cl = (oneCacheLine_t *)_t2->tail->val;
        cl->value = -1; //删除缓存对象的缓存
        assert(cl->list == _t2);
        move_to(_t2->tail, _b1); //将lru1中尾部的缓存对象标记加入ghost_lru1链表中
    }
    current_size--;
}
