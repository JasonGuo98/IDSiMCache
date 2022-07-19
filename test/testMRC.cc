#include "mrc_sim.hh"
#include "normal_trace.hh"
#include "LRU.hh"
#include "LFU.hh"

#include <iostream>
using namespace std;

int main()
{
    const char *trace_name = "../data/zipf_trace.txt";
    // TraceLoader * loader = new NormalTrace(READ_ALL);
    TraceLoader *loader = new NormalTrace(BUFFER_READ);

    loader->read_file(trace_name, 10000);
    Cache *cache = new LFUCache(4);

    MRC_SIM mrc_sim(2, 1, 10);

    mrc_sim.bind_data_cache(loader, cache);

    auto all_mr = mrc_sim.run_MRC();

    for (auto one_mr : all_mr)
    {
        printf("cache_size %ld, hit cnt %ld, hit size %ld, total_req_num %ld\n", one_mr[0], one_mr[1], one_mr[2], mrc_sim.total_req_num);
    }
    return 0;
}