#include<iostream>
#include"normal_trace.hh"
#include"trace_basic_anal.hh"

using namespace std;

int main()
{
    const char * trace_name = "data/zipf_trace.txt";
    // TraceLoader * loader = new NormalTrace(READ_ALL);
    TraceLoader * loader = new NormalTrace(BUFFER_READ);

    loader->read_file(trace_name, 100000);

    uint64_t it = 0, it_size = 0;
    uint64_t cnt = 0;
    while(loader->next_item(it, it_size))
    {
        cout<<cnt++<<": "<<it<<endl;
    }

    loader->reset();

    BasicAnalyzer banal(VIR_TIMER, loader);

    banal.print_info(20);

    if(loader->close_file() == 0)
    {
        printf("close loader OK\n");
    }
    return 0;
}