#pragma once
#include "cache.hh"

// void clFree(void * cl){
//     free((oneCacheLine_t*)(cl));
// }

class LRUCache : virtual public Cache
{
public:
    const char *algoname = "LRUCache";
    int64_t cache_capacity;
    int64_t current_size;
    int64_t current_access_count;

    LRUCache(int64_t cache_capacity) : cache_capacity(cache_capacity),
                                       current_size(0),
                                       current_access_count(0)
    {
        list_p = list_new();
    };

    virtual int64_t set(int64_t, int64_t);

    virtual int64_t resize(int64_t v);

    virtual int64_t get(int64_t, void *);

    virtual int64_t next();

    virtual int64_t reset(int64_t);

    virtual ~LRUCache();

private:
    unordered_map<int64_t, list_node_t *> lookup_table;
    list_t *list_p;

    int64_t pop_one_last_item();
};