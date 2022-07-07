#include"mrc_sim.hh"
#include"normal_trace.hh"
#include"lru.hh"

#include<iostream>
using namespace std;


int main()
{
    // const char * trace_name = "data/zipf_trace.txt";

    const char * filename = "data/zipf_trace.txt";
    int64_t max_trace_len = 10000;
    int64_t cache_size_start = 1;
    double size_factor = 2;
    int64_t n_test_points = 10;

    // TraceLoader * loader = new NormalTrace(READ_ALL);
    TraceLoader * loader = new NormalTrace(BUFFER_READ);

    loader->read_file(filename, max_trace_len);
    Cache * cache = new LRUCache(4);

    MRC_SIM mrc_sim(size_factor, cache_size_start, n_test_points);

    mrc_sim.bind_data_cache(loader, cache);

    auto all_mr = mrc_sim.run_MRC();

    for (auto one_mr : all_mr)
    {
        printf("cache_size %lu, hit cnt %lu, hit size %lu\n", one_mr[0], one_mr[1], one_mr[2]);
    }
    return 0;
}