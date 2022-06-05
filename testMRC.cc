#include"mrc_sim.hh"
#include"normal_trace.hh"
#include"lru.hh"

#include<iostream>
using namespace std;


int main()
{
    const char * trace_name = "data/zipf_trace.txt";
    // TraceLoader * loader = new NormalTrace(READ_ALL);
    TraceLoader * loader = new NormalTrace(BUFFER_READ);

    loader->read_file(trace_name, 10000);
    Cache * cache = new LRUCache(4);

    MRC_SIM mrc_sim(2, 1, 10);

    mrc_sim.bind_data_cache(loader, cache);

    auto all_mr = mrc_sim.run_MRC();

    for (auto one_mr : all_mr)
    {
        printf("cache_size %lu, hit cnt %lu, hit size %lu\n", one_mr[0], one_mr[1], one_mr[2]);
    }
    return 0;
}