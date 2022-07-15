#include "LRU.hh"
#include<iostream>
using namespace std;

int main()
{
    Cache * cache = new LRUCache(4);
    void * cache_line = NULL;

    for( int i = 0; i < 100; i++)
    {
        int it = i % 4;

        if (i == 50) cache->reset();
        auto r = cache->get(it, cache_line);
        cout<<"try get: "<<it<<" result: "<<r<<endl;
        cache->set(it,it);
        r = cache->get(it, cache_line);
        cout<<"try get: "<<it<<" result: "<<r<<endl;
    }
    return 0;
}

