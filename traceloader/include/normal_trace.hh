#pragma once
#include"traceloader.hh"

class NormalTrace: virtual public TraceLoader
{
    public:
    
    LoaderType type;
    FILE * trace_FD;
    uint64_t * buffer;
    vector<uint64_t> all_trace_vec;
    uint64_t buffer_capacity;
    uint64_t max_io_num;
    uint64_t passed_io_cnt;

    NormalTrace(LoaderType t, uint64_t _buffer_capacity = 4096):
    type(t),
    trace_FD(NULL),
    buffer_capacity(_buffer_capacity),
    buffer_idx(0),
    buffer_size(0),
    passed_io_cnt(0),
    max_io_num(0)
    {
        buffer = NULL;
        if (t == BUFFER_READ)
        {
            assert(_buffer_capacity > 0);
            buffer = (uint64_t * )malloc(sizeof(uint64_t)*buffer_capacity);
            if (! buffer)
            {
                log_err(logger, "malloc %ld bytes error\n", sizeof(uint64_t)*buffer_capacity);
            }
            memset(buffer,0, sizeof(uint64_t)*buffer_capacity);
        }
        else if(t == READ_ALL)
        {
            all_trace_vec.clear();
        }
    };

    virtual uint64_t read_file(const char *, uint64_t);

    virtual uint64_t close_file();

    virtual uint64_t next_item(uint64_t & it, uint64_t & it_size);

    virtual uint64_t reset();

    virtual ~NormalTrace()
    {
        if(trace_FD)
            close_file();
        
        if (buffer)
            free(buffer);
    };
private:
    uint64_t buffer_idx;
    uint64_t buffer_size;
    uint64_t _buffer_read();
};