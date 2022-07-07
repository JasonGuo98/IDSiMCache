#pragma once
#include "cache.hh"
#include "traceloader.hh"
#include "utils.hh"
#include "LRU.hh"
#include "normal_trace.hh"

class MRC_SIM
{
public:
    TraceLoader * tl_p;
    Cache * cache_p;
    double mul_factor;
    uint64_t start_anal_cache_size;
    uint64_t max_test_points;

    uint64_t total_req_num;
    uint64_t unique_req_num;
    uint64_t unique_req_size;
    uint64_t total_req_size;

    bool ready_to_sim;


    MRC_SIM(double mul_factor = 2, uint64_t start_anal_cache_size = 1, uint64_t max_test_points = 20):
    cache_p(NULL),
    tl_p(NULL),
    mul_factor(mul_factor),
    start_anal_cache_size(start_anal_cache_size),
    max_test_points(max_test_points)
    {
        unique_req_num = 0;
        total_req_num = 0;
        unique_req_size = 0;
        total_req_size = 0;
        ready_to_sim = false;
    };

    uint64_t bind_data_cache(TraceLoader * _tl_p, Cache * _cache_p );
    // only for homogeneous objects
    vector<vector<int64_t>> run_MRC();

private:
    void _basic_info();

    
    pair<int64_t, int64_t> _one_test(uint64_t);

};