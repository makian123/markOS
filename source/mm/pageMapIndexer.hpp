#pragma once

#include <include/types.hpp>

class PageMapIndexer{
    public:
        PageMapIndexer(uint64_t virtAddress);
        uint64_t PDP_i;
        uint64_t PD_i;
        uint64_t PT_i;
        uint64_t P_i;
};