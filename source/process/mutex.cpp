#include "mutex.hpp"

void Mutex::Lock(){
    while(!__sync_bool_compare_and_swap(&locked, false, true));
}

void Mutex::Unlock(){
    __sync_bool_compare_and_swap(&locked, true, false);
}

bool Mutex::GetLock(){
    return __sync_bool_compare_and_swap(&locked, true, true);
}