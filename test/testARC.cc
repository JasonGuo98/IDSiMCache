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
    cout << 111 << endl;
    void *cache_line = NULL;
    cache->reset();
    cout << 111 << endl;
    try_get(cache, 1, cache_line);
    cache->set(1, 111);
    try_get(cache, 1, cache_line);
    try_get(cache, 2, cache_line);
    cache->set(2, 222);
    try_get(cache, 2, cache_line);
    try_get(cache, 3, cache_line);
    cache->set(3, 333);
    try_get(cache, 1, cache_line);
    try_get(cache, 4, cache_line);
    cache->set(4, 444);
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
