#pragma once
#include "utils.hh"
#include"murmur3.h"
#include"list.h"
#include <unordered_map>
#include <stdint.h>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstddef>

using namespace std;


class Cache
{
public:
    Cache() {};

    virtual uint64_t reset(uint64_t v = 0) = 0;

    virtual uint64_t set(uint64_t, uint64_t) = 0;

    virtual uint64_t resize(uint64_t) = 0;

    virtual uint64_t get(uint64_t, void *) = 0;

    virtual uint64_t next() = 0;

    virtual ~Cache(){};
};


