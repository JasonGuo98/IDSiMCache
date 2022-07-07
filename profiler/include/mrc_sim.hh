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
    int64_t start_anal_cache_size;
    int64_t max_test_points;

    int64_t total_req_num;
    int64_t unique_req_num;
    int64_t unique_req_size;
    int64_t total_req_size;

    bool ready_to_sim;


    MRC_SIM(double mul_factor = 2, int64_t start_anal_cache_size = 1, int64_t max_test_points = 20):
    tl_p(NULL),
    cache_p(NULL),
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

    int64_t bind_data_cache(TraceLoader * _tl_p, Cache * _cache_p );
    // only for homogeneous objects
    vector<vector<int64_t>> run_MRC();

private:
    void _basic_info();

    
    pair<int64_t, int64_t> _one_test(int64_t);

};