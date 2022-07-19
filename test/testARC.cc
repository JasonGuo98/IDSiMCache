#include "ARC.hh"
#include <iostream>
using namespace std;

void try_get(Cache *cache, int key, void *p)
{
    auto r = cache->get(key, p);
    cout << "try get: " << key << " result: " << r << endl;
}

int main()
{
    Cache *cache = new ARCCache(3);
    void *cache_line = NULL;
    cache->reset();
    try_get(cache, 1, cache_line);
    cache->set(1, 1);
    try_get(cache, 1, cache_line);
    try_get(cache, 2, cache_line);
    cache->set(2, 2);
    try_get(cache, 2, cache_line);
    try_get(cache, 3, cache_line);
    cache->set(3, 3);
    try_get(cache, 1, cache_line);
    try_get(cache, 4, cache_line);
    cache->set(4, 4);
    try_get(cache, 1, cache_line);
    try_get(cache, 2, cache_line);
    try_get(cache, 3, cache_line);
    try_get(cache, 4, cache_line);

    // for (int i = 0; i < 100; i++)
    // {
    //     int it = i % 4;

    //     if (i == 50)
    //         cache->reset();
    //     auto r = cache->get(it, cache_line);
    //     cout << "try get: " << it << " result: " << r << endl;
    //     cache->set(it, it);
    //     r = cache->get(it, cache_line);
    //     cout << "try get: " << it << " result: " << r << endl;
    // }
    return 0;
}
