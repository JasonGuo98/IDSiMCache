#pragma once
#include "cache.hh"

class LFUCache : virtual public Cache //虚基类
{
private:
    unordered_map<int64_t, list_node_t *> obj_table;
    unordered_map<int64_t, list_t *> freq_table;

    int64_t pop_one_last_item();

public:
    const char *algoname = "LFUCache";
    int64_t cache_capacity; //缓存空间大小
    int64_t current_size;   //当前缓存大小
    int64_t current_access_count;
    int64_t min_freq;

    LFUCache(int64_t cache_capacity) : cache_capacity(cache_capacity),
                                       current_size(0),
                                       current_access_count(0)
    {
        min_freq = 1;
    };

    LFUCache(const LFUCache &) = delete;            //禁止拷贝构造
    LFUCache &operator=(const LFUCache &) = delete; //禁止拷贝赋值

    virtual int64_t set(int64_t, int64_t);

    virtual int64_t resize(int64_t v);

    virtual int64_t get(int64_t, void *);

    virtual int64_t next();

    virtual int64_t reset(int64_t);

    virtual ~LFUCache()
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
    };
};