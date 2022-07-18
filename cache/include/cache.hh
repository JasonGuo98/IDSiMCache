#pragma once
#include "utils.hh"
#include "murmur3.h"
#include "list.h"
#include <unordered_map>
#include <stdint.h>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstddef>

using namespace std;

struct oneCacheLine_t
{
    int64_t key;
    int64_t value;
    int64_t freq;
    int64_t prio;
    list_t *list = nullptr;
    // char buffer[1];
};

class Cache
{
public:
    Cache(){};

    virtual int64_t reset(int64_t v = 0) = 0;

    virtual int64_t set(int64_t, int64_t) = 0;

    virtual int64_t resize(int64_t) = 0;

    virtual int64_t get(int64_t, void *) = 0;

    virtual int64_t next() = 0;

    virtual ~Cache(){};
};
