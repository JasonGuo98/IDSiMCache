#include "LFU.hh"

void myfree(void *val)
{
    assert(val);
    free(val);
}

int64_t LFUCache::reset(int64_t new_capacity = 0)
{
    obj_table.clear();
    if (freq_table.size())
    {
        auto it = freq_table.begin();
        while (it != freq_table.end())
        {
            list_t *temp = it->second;
            freq_table.erase(it++);
            list_destroy(temp);
        }
    }
    current_size = 0;
    current_access_count = 0;
    if (new_capacity != 0)
    {
        cache_capacity = new_capacity;
    }
    return 0;
}

int64_t LFUCache::resize(int64_t target_capacity)
{
    assert(target_capacity > 0);

    while (current_size > target_capacity) //删除缓存对象直到不超过容量上限为止
    {
        assert(pop_one_last_item());
        while (current_size && (!freq_table[min_freq] || freq_table[min_freq]->len == 0))
            min_freq++; //若删除之后对应频率的链表已经为空，则更新最小使用频率
    }

    cache_capacity = target_capacity;
    return 0;
}

int64_t LFUCache::get(int64_t key, void *p)
{

    if (cache_capacity == 0)
        return -1;
    auto iter = obj_table.find(key); //通过key-缓存对象的哈希表找到缓存对象
    if (iter == obj_table.end())     //没找到，返回-1
        return -1;
    list_node_t *node = iter->second; //获取缓存对象指针
    oneCacheLine_t *cl = (oneCacheLine_t *)node->val;
    int freq = cl->freq;                 //获取缓存对象的值以及访问频率
    list_remove(freq_table[freq], node); //在对应访问频率的双向链表中删除缓存对象
    if (freq_table[freq]->len == 0)
    { //如果当前访问频率的链表已经为空，我们需要在哈希表中删除此频率的双向链表，且更新minFreq
        list_destroy(freq_table[freq]);
        freq_table.erase(freq);
        if (min_freq == freq)
            min_freq += 1;
    }
    if (!freq_table[freq + 1]) //若没有访问频率为freq+1的链表，则新建
    {
        freq_table[freq + 1] = list_new();
        freq_table[freq + 1]->free = myfree;
    }
    // 更新此缓存对象的访问次数，并将其插入到访问频率为 freq + 1 的链表中
    cl->freq += 1;
    list_lpush(freq_table[freq + 1], node);
    obj_table[key] = freq_table[freq + 1]->head; //更新key-缓存对象 哈希表
    return cl->value;
}

int64_t LFUCache::set(int64_t key, int64_t value = 0)
{
    if (cache_capacity == 0)
        return 0;
    auto it = obj_table.find(key); //查找缓存列表中是否已经存在当前缓存对象
    if (it == obj_table.end())     //没找到，则插入
    {
        // 缓存已满，需要进行删除操作
        if (obj_table.size() == cache_capacity)
        { //删除使用频率最少的节点中最近最远未使用的那个

            pop_one_last_item();
            //此处不用更新最小使用频率，因为后面要新插入使用次数为1的节点
            //因此最小使用频率一定会为1
        }
        // oneCacheLine_t *cl = (oneCacheLine_t *)malloc(sizeof(oneCacheLine_t));
        oneCacheLine_t *cl = new oneCacheLine_t{key, value, 1, current_access_count};
        assert(cl != NULL);
        // cl->key = key;
        // cl->value = value;
        // cl->prio = current_access_count;
        list_node_t *node_p = list_node_new(cl);
        if (!freq_table[1])
        {
            freq_table[1] = list_new();
            freq_table[1]->free = myfree;
        }
        list_lpush(freq_table[1], node_p);    //在使用频率为1的链表中插入当前节点
        obj_table[key] = freq_table[1]->head; //更新key-缓存对象哈希表
        min_freq = 1;                         //最小使用频率为1
        current_size++;
    }
    else //缓存中已经存在当前缓存对象，则将其值和使用频率更新
    {
        // 与 get 操作基本一致，除了需要更新缓存的值和使用频率
        list_node_t *node = it->second;
        oneCacheLine_t *cl = (oneCacheLine_t *)node->val;
        int freq = cl->freq;
        list_remove(freq_table[freq], node);
        if (freq_table[freq]->len == 0)
        {
            list_destroy(freq_table[freq]);
            freq_table.erase(freq);
            if (min_freq == freq)
                min_freq += 1;
        }
        if (!freq_table[freq + 1]) //若没有访问频率为freq+1的链表，则新建
        {
            freq_table[freq + 1] = list_new();
            freq_table[freq + 1]->free = myfree;
        }
        // 更新此缓存对象的访问次数，并将其插入到访问频率为 freq + 1 的链表中
        cl->freq += 1;
        list_lpush(freq_table[freq + 1], node);
        obj_table[key] = freq_table[freq + 1]->head; //更新key-缓存对象 哈希表
        return cl->value;
    }
    return 0;
}

int64_t LFUCache::next()
{
    current_access_count++;
    return current_access_count;
}

int64_t LFUCache::pop_one_last_item()
{
    //删除使用频率最少的节点中最近最远未使用的那个
    assert(freq_table[min_freq]->len > 0);
    list_node_t *lfu_item = list_rpop(freq_table[min_freq]); // 通过 min_freq 拿到 freq_table[min_freq] 链表的末尾节点
    if (freq_table[min_freq]->len == 0)
    { //若当前访问频率的链表在删除节点之后已经为空，则删除当前链表
        list_destroy(freq_table[min_freq]);
        freq_table.erase(min_freq);
        // min_freq之后更新成哪个值要看情况，共有两种情况会调用此函数进行缓存对象的删除：
        // 1、缓存空间满了，要进行替换，此时在删除缓存对象之后还会再加入新的缓存对象，min_freq会被更新为1
        // 2、清空缓存，min_freq要更新为新的最小访问频率
    }

    assert(lfu_item);
    oneCacheLine_t *del_cl = (oneCacheLine_t *)(lfu_item->val);

    int64_t del_key = del_cl->key;
    auto iter = obj_table.find(del_key);
    assert(iter != obj_table.end());
    obj_table.erase(iter); //在key-缓存对象哈希表删除相应键值对
    free(del_cl);
    free(lfu_item);
    current_size--;

    return 1;
}
