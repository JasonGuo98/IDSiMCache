#include "normal_trace.hh"

uint64_t NormalTrace::read_file(const char * filename, uint64_t _max_io_num = 0)
{
    if(trace_FD)
    {
        log_err(logger, "already open one trace file\n");
        exit(1);
    }
    trace_FD = fopen(filename, "r");
    
    if (! trace_FD)
    {
        log_fatal(logger,"open file %s failed\n", filename);
        exit(1);
    }

    if (type == BUFFER_READ)
    {
        if(!_buffer_read())
        {
            log_warn(logger, "no IO in file: %s\n", filename);
        }
    }
    else if(type == READ_ALL)
    {
        uint64_t it;
        uint64_t last_size = all_trace_vec.size();
        while(fscanf(trace_FD, "%lu", &it) != EOF)
        {
            all_trace_vec.push_back(it);
            if(all_trace_vec.size() == last_size)
            {
                log_err(logger, "vector push back fail, vec size %ld\n", last_size);
                assert(0);
            }
            last_size = all_trace_vec.size();
        }
        buffer_size = all_trace_vec.size();
        buffer_idx = 0;
    }

    max_io_num = _max_io_num;
    passed_io_cnt = 0;
    return 0;
}

uint64_t NormalTrace::close_file()
{
    if (! trace_FD)
    {
        log_err(logger, "close trace file error\n");
        assert(trace_FD);
    }

    fclose(trace_FD);
    trace_FD = NULL;

    if (type == BUFFER_READ)
    {
        ;
    }
    else{
        all_trace_vec.clear();
    }

    buffer_size = 0;
    buffer_idx = 0;

    max_io_num = 0;
    passed_io_cnt = 0;

    return 0;
}

uint64_t NormalTrace::_buffer_read()
{
    if (!trace_FD)
    {
        log_err(logger, "no open trace file\n");
        assert(trace_FD);
    }

    if (buffer_idx != buffer_size)
    {
        log_warn(logger, "not finish buffer trace IO, buffer_size: %lu, buffer_idx: %lu\n", buffer_size, buffer_idx);
    }

    buffer_size = 0;
    buffer_idx = 0;
    uint64_t it;
    while(buffer_size < buffer_capacity && fscanf(trace_FD, "%lu", &it) != EOF)
    {
        buffer[buffer_size++] = it;
    }
    if (buffer_size == 0)
    {
        log_info(logger, "read 0 item from trace file!\n");
    }
    return buffer_size;
}

uint64_t NormalTrace:: next_item(uint64_t & it, uint64_t & it_size)
{
    if (max_io_num && passed_io_cnt >= max_io_num)
    {
        log_info(logger, "reach max io threshold: %lu\n", max_io_num);
        return 0;
    }

    if (!trace_FD)
    {
        log_err(logger, "no open trace file\n");
        assert(trace_FD);
    }
    it_size = 1;

    if (type == BUFFER_READ)
    {
        if(buffer_idx < buffer_size)
        {
            it = buffer[buffer_idx++];
            passed_io_cnt++;
            return passed_io_cnt;
        }
        else if(_buffer_read())
        {
            it = buffer[buffer_idx++];
            passed_io_cnt++;
            return passed_io_cnt;
        }
        else
        {
            log_info(logger, "No IO in buffer read file\n");
            return 0;
        }
    }
    else if (type == READ_ALL)
    {
        if(buffer_idx < buffer_size)
        {
            it = all_trace_vec[buffer_idx++];
            passed_io_cnt++;
            return passed_io_cnt;
        }
        else
        {
            log_info(logger, "all IO passed\n");
            return 0;
        }
    }
    return 0;
}

uint64_t NormalTrace::reset()
{
    if (!trace_FD)
    {
        log_err(logger, "no open trace file\n");
        assert(trace_FD);
    }

    if (type == BUFFER_READ)
    {
        fseek(trace_FD,0,SEEK_SET);
        if(! _buffer_read())
        {
            log_err(logger, "Read IO buffer after fseek failed\n");
        }
        passed_io_cnt = 0;
    }
    else if (type == READ_ALL)
    {
        buffer_idx = 0;
        passed_io_cnt = 0;
    }
    return 0;
}