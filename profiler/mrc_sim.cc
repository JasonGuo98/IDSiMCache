#include "mrc_sim.hh"

void MRC_SIM::_basic_info()
{
    if(tl_p->reset()) log_err(logger, "reset trace loader error\n");
    uint64_t it = 0, it_size = 0;
    unique_req_num = 0;
    total_req_num = 0;
    unique_req_size = 0;
    total_req_size = 0;
    unordered_map<uint64_t, uint64_t> obj_cnt;
    while(tl_p->next_item(it, it_size))
    {
        total_req_num++;
        total_req_size += it_size;
        if(obj_cnt.find(it) == obj_cnt.end())
        {
            unique_req_num++;
            unique_req_size += it_size;
            obj_cnt[it] = 1;
        }
    }
    if(tl_p->reset()) log_err(logger, "reset trace loader error\n");
}

pair<uint64_t, uint64_t> MRC_SIM::_one_test(uint64_t cache_size)
{
    uint64_t hit_num = 0;
    uint64_t hit_size = 0;
    cache_p->reset(cache_size);
    if(tl_p->reset()) log_err(logger, "reset trace loader error\n");

    uint64_t it = 0, it_size = 0;
    uint64_t cnt = 0;
    while(tl_p->next_item(it, it_size))
    {
        cnt++;
        if(cache_p->get(it, NULL))
        {
            hit_num++;
            hit_size += it_size;
        }
        else
        {
            cache_p->set(it,it_size);
        }
        cache_p->next();
    }
    auto ret = make_pair(hit_num, hit_size);
    return ret;
}

vector<vector<uint64_t>> MRC_SIM::run_MRC()
{
    if(!ready_to_sim)
    {
        log_err(logger,"not ready to sim\n");
        assert(0);
    }
    vector<vector<uint64_t>> results_hit_cnt;
    uint64_t _cache_size = start_anal_cache_size;
    uint64_t next_cache_size = start_anal_cache_size;
    uint64_t points_done = 0;
    do
    {
        _cache_size = next_cache_size;
        auto ret = _one_test(_cache_size);
        vector<uint64_t> ret_c_size;
        ret_c_size.push_back(_cache_size);
        ret_c_size.push_back(ret.first);
        ret_c_size.push_back(ret.second);
        results_hit_cnt.push_back(ret_c_size);

        next_cache_size = _cache_size*mul_factor;
        if (next_cache_size == _cache_size) next_cache_size = _cache_size + 1;
        points_done++;

    }while(next_cache_size < total_req_size && points_done < max_test_points);

    return results_hit_cnt;
}

uint64_t MRC_SIM::bind_data_cache(TraceLoader * _tl_p, Cache * _cache_p)
{
    
    if(!_tl_p) {log_err(logger, "No trace loader\n"); exit(1);}
    if(!_cache_p) {log_err(logger, "No cache instance\n");exit(1);}
    tl_p = _tl_p;
    cache_p = _cache_p;
    _basic_info();
    ready_to_sim = true;

    printf("total_req_num: %ld, total_req_size: %ld\n", total_req_num, total_req_size);
    printf("unique_req_num: %ld, unique_req_size: %ld\n", unique_req_num, unique_req_size);
    return 0;
}