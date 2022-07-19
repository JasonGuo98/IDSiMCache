#pragma once
#include "traceloader.hh"

struct block_trace_line
{
    int starting_block = -1;
    int number_of_blocks = -1;
    int ignore;
    int request_number = -1;

    /**
    Example: first line in P6.lis is
    110765 64 0 0

    110765	starting block
    64		64 blocks each of 512 bytes
            so this represents 64 requests (each of a 512 byte page) from 110765 to 110828
    0		ignore
    0		request number (goes from 0 to n-1)
    */
};

class BlockTrace : virtual public TraceLoader
{
public:
    LoaderType type;

    FILE *trace_FD;

    int64_t passed_io_cnt = 0;

    int64_t max_io_num;

    block_trace_line current_line;

    int64_t current_block = -1;

    BlockTrace(LoaderType t) : type(t),
                               trace_FD(NULL),
                               max_io_num(0x3f3f3f3f),
                               passed_io_cnt(0){};

    virtual int64_t read_file(const char *, int64_t);

    virtual int64_t close_file();

    /**
     * @brief store next item key in &it, and return virtual time or physical time of this IO
     *
     * @param it
     * @return int64_t
     */
    virtual int64_t next_item(int64_t &it, int64_t &it_size);

    virtual int64_t reset();

    virtual ~BlockTrace()
    {
        if (trace_FD)
            close_file();
    };
};