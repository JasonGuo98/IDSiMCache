#pragma once
#include"cache.hh"


struct oneCacheLine_t{
    uint64_t key;
    uint64_t value;
    uint64_t prio;
    // char buffer[1];
} ;

// void clFree(void * cl){
//     free((oneCacheLine_t*)(cl));
// }


class LRUCache: virtual public Cache
{
public:
    const char* algoname = "LRUCache";
    uint64_t cache_capacity;
    uint64_t current_size;
    uint64_t current_access_count;


    LRUCache(uint64_t cache_capacity):
        cache_capacity(cache_capacity), 
        current_size(0),
        current_access_count(0)
    {
        list_p = list_new();
    };

    virtual uint64_t set(uint64_t ,  uint64_t );

    virtual uint64_t get(uint64_t, void * );

    virtual uint64_t next();

    virtual uint64_t reset(uint64_t );

    virtual ~LRUCache();



private:
    unordered_map<uint64_t, list_node_t * > lookup_table;
    list_t * list_p;
};