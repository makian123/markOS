#include "lib/malloc.h"

uint8_t memory[MEMORY_SIZE];

struct block *freeList = (void*) memory;

void Initialize(){
    freeList->size = (MEMORY_SIZE) - sizeof(struct block);
    freeList->free = 1;
    freeList->next = null;
}

void Merge(){
    struct block *prev, *curr;

    curr = freeList;

    while(curr->next->next != null){
        if(curr && curr->next && curr->next->next){
            curr->size += curr->next->size + sizeof(struct block);
            curr->next = curr->next->next;
        }
        
        prev = curr;
        curr = curr->next;
    }
}

void Split(struct block *fSlot, size_t size){
    struct block *new = (void*)((void*) fSlot+size+sizeof(struct block));
    new->size = (fSlot->size) - size - sizeof(struct block);
    new->free = 1;
    new->next = fSlot->next;
    fSlot->size = size;
    fSlot->free = 0;
    fSlot->next = new;
}

void *Malloc(size_t size){
    struct block *prev, *curr;
    void *result;

    if(!(freeList->size)) Initialize();
    curr = freeList;

    while( ((curr->size < size) || curr->free == 0) && curr->next != null){
        prev = curr;
        curr = curr->next;
    }

    if(curr ->size == size){
        curr->free = 0;
        result = (void*)++curr;
        return result;
    }
    else if(curr->size > (size + sizeof(struct block))){
        Split(curr, size);
        result = (void*)++curr;
        return result;
    }
    else{
        result = null;
        return result;
    }
}

void Free(void *ptr){
     if((void*)memory <= ptr && ptr <= (void*)(memory + 20000)){
        struct block *curr = ptr;
        --curr;
        curr->free = 1;
    }
}