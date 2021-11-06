#include "pageMapIndexer.hpp"

PageMapIndexer::PageMapIndexer(uint64_t virtAddress){
    virtAddress >>= 12;
    P_i = virtAddress & 0x1FF;
    virtAddress >>= 9;
    PT_i = virtAddress & 0x1FF;
    virtAddress >>= 9;
    PD_i = virtAddress & 0x1FF;
    virtAddress >>= 9;
    PDP_i = virtAddress & 0x1FF;
}