#pragma once
#include"traceloader.hh"

class NormalTrace: virtual public TraceLoader
{
    public:
    
    LoaderType type;
    FILE * trace_FD;
    int64_t * buffer;
    vector<int64_t> all_trace_vec;
    int64_t buffer_capacity;
    int64_t max_io_num;
    int64_t passed_io_cnt;
    

    NormalTrace(LoaderType t, int64_t _buffer_capacity = 4096):
    type(t),
    trace_FD(NULL),
    buffer_capacity(_buffer_capacity),
    max_io_num(0),
    passed_io_cnt(0),
    buffer_idx(0),
    buffer_size(0)
    {
        buffer = NULL;
        if (t == BUFFER_READ)
        {
            assert(_buffer_capacity > 0);
            buffer = (int64_t * )malloc(sizeof(int64_t)*buffer_capacity);
            if (! buffer)
            {
                log_err(logger, "malloc %ld bytes error\n", sizeof(int64_t)*buffer_capacity);
            }
            memset(buffer,0, sizeof(int64_t)*buffer_capacity);
        }
        else if(t == READ_ALL)
        {
            all_trace_vec.clear();
        }
    };

    virtual int64_t read_file(const char *, int64_t);

    virtual int64_t close_file();

    virtual int64_t next_item(int64_t & it, int64_t & it_size);

    virtual int64_t reset();

    virtual ~NormalTrace()
    {
        if(trace_FD)
            close_file();
        
        if (buffer)
            free(buffer);
    };
private:
    int64_t buffer_idx;
    int64_t buffer_size;
    int64_t _buffer_read();
};