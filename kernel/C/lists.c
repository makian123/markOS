#include "lib/lists.h"
#include "lib/malloc.h"

struct List{
    long val;
    struct list *prev;
    struct list *next;
};

void ListCreate(struct List *head, long value){
    head->val = value;
}

void ListAdd(struct List *head, long toAdd){
    while(head->next != null){
        head = head->next;
    }
    struct List *previous = &head;
    head->next = (struct List*) Malloc(sizeof(struct List*));
    head = head->next;
    head->prev = previous;
    head->val = toAdd;
    previous = null;
}

long ListGetAt(struct List *head, size_t pos){
    while(pos > 0){
        head = head->next;
        pos--;
    }
    return head->val;
}

void ListInsert(struct List *head, size_t pos, long val){
    while(pos > 0){
        head = head->next;
        pos--;
    }

    struct List *nextOne = head->next;

    head->next = (struct List*) Malloc(sizeof(struct List));

    struct List *previous = head;

    head = head->next;
    head->val = val;
    head->prev = previous;
    head->next = nextOne;
}

struct List *ListNext(struct List *node){
    return node->next;
}

long ListGetValue(struct List *node){
    return node->val;
}