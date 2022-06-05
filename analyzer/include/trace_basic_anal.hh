#pragma once
#include "utils.hh"
#include "traceloader.hh"
#include <unordered_map>
using namespace std;

enum TimerType_t{REAL_TIMER, VIR_TIMER};

class BasicAnalyzer
{
public:
    uint64_t t_start, t_end;
    uint64_t total_req, unique_req;
    double average_req_cnt;
    double medium_req_cnt;
    uint64_t max_req_cnt;
    uint64_t min_req_cnt;

    TimerType_t timer_type;
    TraceLoader * tl_p;

    unordered_map<uint64_t, uint64_t> req_map;


    BasicAnalyzer(TimerType_t _timer_type, TraceLoader * tl):
    timer_type(_timer_type),
    tl_p(tl)
    {
        t_start = 0;
        t_end = 0;
        total_req = 0;
        unique_req = 0;
        average_req_cnt = 0;
        medium_req_cnt = 0;
        max_req_cnt = 0;
        min_req_cnt = 0;
    }
};




