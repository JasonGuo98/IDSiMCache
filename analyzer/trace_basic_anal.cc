#include"trace_basic_anal.hh"

/*
dat: shorter_xaa
number of requests: 668960
number of uniq obj/blocks: 514987
cold miss ratio: 0.7698
top N popular (obj, num of requests): 
[(b'5999360', 1856),
 (b'5999368', 1851),
 (b'6229512', 637),
 (b'6229520', 637),
 (b'833040', 525),
 (b'1253128', 506),
 (b'1253120', 503),
 (b'0', 495)]
number of obj/block accessed only once: 459198
frequency mean: 1.30
*/

void BasicAnalyzer::print_info(uint64_t n_top_hot_item)
{
    uint64_t it = 0, it_size = 0;

    t_start = 0;

    while(tl_p->next_item(it, it_size))
    {
        total_req++;
        

        if(req_cnt_map.count(it) == 0){
            unique_req++;
            req_cnt_map[it] = 1;
        }
        else{
            req_cnt_map[it] ++;
        }
    }

    typedef pair<uint64_t, uint64_t> id_cnt_t;
    auto cmp_func = [](const id_cnt_t & a,const id_cnt_t & b){return a.second > b.second;};

    vector<id_cnt_t> req_cnt_kv;
    for(auto kv: req_cnt_map)
    {
        req_cnt_kv.push_back(kv);
        if(1 == kv.second)
        {
            only_once_req++;
        }
    }

    sort(req_cnt_kv.begin(), req_cnt_kv.end(), cmp_func);

    if (unique_req % 2 == 0)
    {
        medium_req_cnt = (req_cnt_kv[unique_req / 2 - 1].second + req_cnt_kv[unique_req / 2].second) / 2;
    }
    else 
    {
        medium_req_cnt = req_cnt_kv[unique_req / 2].second;
    }

    average_req_cnt = (1.0*total_req)/unique_req;

    min_req_cnt = req_cnt_kv[0].second;
    max_req_cnt = req_cnt_kv[unique_req-1].second;

    /**** print all info ****/

    printf("data set name: %s\n", (tl_p->trace_name).c_str());
    printf("number of requests: %ld\n", total_req);
    printf("number of uniq obj/blocks: %ld\n", unique_req);
    printf("cold miss ratio: %lf\n", (1.0*unique_req)/total_req);
    printf("top [%ld] popular (obj, num of requests):\n", min(n_top_hot_item, unique_req));

    for(int i = 0; i < unique_req && i < n_top_hot_item; i++)
    {
        printf("\t ( %ld, %ld )\n", req_cnt_kv[i].first, req_cnt_kv[i].second);
    }
    
    printf("number of obj/block accessed only once: %ld\n", only_once_req);
    printf("average access frequency: %lf\n", average_req_cnt);
    printf("medium_req_cnt access frequency: %lf\n", medium_req_cnt);
}