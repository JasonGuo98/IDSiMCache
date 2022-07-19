#include "block_trace.hh"

int64_t BlockTrace::read_file(const char *filename, int64_t _max_io_num)
{
    if (trace_FD) //已经打开过文件了
    {
        log_err(logger, "already open one trace file\n");
        exit(1);
    }
    trace_name = filename;

    trace_FD = fopen(filename, "r");

    if (!trace_FD) //打开文件失败
    {
        log_fatal(logger, "open file %s failed\n", filename);
        exit(1);
    }

    max_io_num = (_max_io_num == -1 ? 0x3f3f3f3f : _max_io_num); //-1则表示不限制读取的数据量
    return 0;
}

int64_t BlockTrace::close_file()
{
    if (!trace_FD) //没有打开过文件
    {
        log_err(logger, "close trace file error\n");
        assert(trace_FD);
    }

    fclose(trace_FD);
    trace_FD = NULL;

    return 0;
}

int64_t BlockTrace::next_item(int64_t &it, int64_t &it_size)
{
    if (!trace_FD)
    {
        log_err(logger, "no open trace file\n");
        assert(trace_FD);
    }
    if (passed_io_cnt % 100000 == 0)
        printf("passed_io_cnt:%d\n", passed_io_cnt);
    if (passed_io_cnt == -1) //已经将文件读完了
    {
        log_info(logger, "all IO passed\n");
        return 0;
    }
    else if (passed_io_cnt == max_io_num) //已经读到设定的数据量了
    {
        log_info(logger, "reach max io threshold: %lu\n", max_io_num);
        return 0;
    }
    if (current_block == -1) //第一次读
    {
        if (fscanf(trace_FD, "%d %d %d %d\n",
                   &current_line.starting_block, &current_line.number_of_blocks, &current_line.ignore, &current_line.request_number) == EOF)
        {
            log_info(logger, "read 0 item from trace file!\n");
            return 0;
        }
        it = current_line.starting_block;
        it_size = 1;
        current_block = current_line.starting_block + 1;
        passed_io_cnt++;
    }
    else //非第一次读
    {
        it = current_block++;
        it_size = 1;
        passed_io_cnt++;
    }
    if (current_block - current_line.starting_block == current_line.number_of_blocks) //继续读下一行
    {
        if (fscanf(trace_FD, "%d %d %d %d\n",
                   &current_line.starting_block, &current_line.number_of_blocks, &current_line.ignore, &current_line.request_number) == EOF)
        {
            passed_io_cnt = -1; //置为-1代表已经读完了
        }
        else
        {
            current_block = current_line.starting_block;
        }
    }
    // printf("current_line:%d %d %d %d\n", current_line.starting_block, current_line.number_of_blocks, current_line.ignore, current_line.request_number);
    return current_line.request_number + 1; //返回request标号(0~n-1)->(1~n)
}

int64_t BlockTrace::reset()
{
    if (!trace_FD)
    {
        log_err(logger, "no open trace file\n");
        assert(trace_FD);
    }
    fseek(trace_FD, 0, SEEK_SET); //文件指针回到起始位置
    passed_io_cnt = 0;
    return 0;
}