#include "lib/lists.h"
#include "lib/malloc.h"

struct List{
    long val;
    struct list *prev;
    struct list *next;
};

void ListAdd(struct List *head, long toAdd){
    while(head->next != null){
        head = head->next;
    }
    struct List *previous = head;
    head->next = (struct List*) Malloc(sizeof(struct List*));
    head = head->next;
    head->prev = previous;
    head->val = toAdd;
    previous = null;
}