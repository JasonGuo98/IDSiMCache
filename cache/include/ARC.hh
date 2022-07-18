#pragma once
#include "cache.hh"

class ARCCache : virtual public Cache //虚基类
{
private:
    list_t *_t1 = nullptr; // lru1
    list_t *_b1 = nullptr; // ghost_lru1
    list_t *_t2 = nullptr; // lru2
    list_t *_b2 = nullptr; // ghost_lru2
    unordered_map<int64_t, list_node_t *> obj_table;
    double t1_p; // lru1链表的预期长度

    void replace(bool in_b2); //从缓存中删除末尾的缓存对象，即将lru1或lru2中的末尾元素删除并插入到ghost_lru1或ghost_lru2中

    void move_to(list_node_t *node, list_t *list); //将缓存对象从一个链表移动到另一个链表

    void assert_c() //必须满足的缓存空间限制条件，ARC算法设定的前提条件
    {
        //维护lru1(只访问过一次的缓存对象),ghost_lru1(从lru1中淘汰的缓存对象key),lru2(访问过超过两次的缓存对象),ghost_lru2(从lru2中淘汰的缓存对象key)
        //这四个双向链表均使用lru进行首部插入和尾部删除
        //算法定义：保证始终满足lru1+lru2的长度不大于缓存空间大小，lru1+ghost_lru1的长度不大于缓存空间大小
        // lru2+ghost_lru2的长度不大于缓存空间大小的两倍，四个链表的总长度不大于缓存空间大小的两倍
        // ARC算法通过这四个链表，基于历史信息动态地在recency和frequency之间做了动态权衡
        assert(_t1->len + _t2->len <= cache_capacity);
        assert(_t1->len + _b1->len <= cache_capacity);
        assert(_t2->len + _b2->len <= cache_capacity * 2);
        assert(_t1->len + _b1->len + _t2->len + _b2->len <= cache_capacity * 2);
    }

public:
    const char *algoname = "ARCCache";
    int64_t cache_capacity; //缓存空间大小
    int64_t current_size;   //当前缓存大小
    int64_t current_access_count;

    ARCCache(int64_t cache_capacity) : cache_capacity(cache_capacity),
                                       current_size(0),
                                       t1_p(0),
                                       current_access_count(0)
    {
        //初始时，四个链表均为空，且在lru1+lru2的总长度达到缓存空间大小之前，ghost_lru1和ghost_lru2会始终为空
        reset(cache_capacity);
    };

    ARCCache(const ARCCache &) = delete;            //禁止拷贝构造
    ARCCache &operator=(const ARCCache &) = delete; //禁止拷贝赋值

    virtual int64_t set(int64_t, int64_t);

    // ARC为非堆栈类算法，减小cache_capability后每次要删除的末尾元素不是确定的，应该无法实现resize
    virtual int64_t resize(int64_t new_capability)
    {
        //直接清空
        reset(new_capability);
        return 0;
    }

    virtual int64_t get(int64_t, void *);

    virtual int64_t next();

    virtual int64_t reset(int64_t);

    virtual ~ARCCache()
    {
        obj_table.clear();
        list_destroy(_t1);
        list_destroy(_b1);
        list_destroy(_t2);
        list_destroy(_b2);
    };
};