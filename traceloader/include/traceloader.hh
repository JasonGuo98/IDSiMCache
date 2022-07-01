#pragma once
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<stdint.h>
#include<assert.h>
#include<vector>
#include "utils.hh"

using namespace std;

enum LoaderType{READ_ALL, BUFFER_READ};
class TraceLoader
{
public:
    string trace_name;
    
    TraceLoader()
    {
        ;
    };

    virtual uint64_t read_file(const char *, uint64_t) = 0;

    virtual uint64_t close_file() = 0;

    /**
     * @brief store next item key in &it, and return virtual time or physical time of this IO
     * 
     * @param it 
     * @return uint64_t 
     */
    virtual uint64_t next_item(uint64_t & it,  uint64_t & it_size) = 0;

    virtual uint64_t reset() = 0;

    virtual ~TraceLoader(){};

};