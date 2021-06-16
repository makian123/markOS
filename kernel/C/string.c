#include "lib/string.h"

void StringCreate(struct string *str, uint8_t *arr){
    size_t index = 0;

    if(arr[1] == null){
        str->data = arr[0];
        str->next = (struct string*) Malloc(sizeof(struct string));
        str->next->data = '\0';
    }
    
    while(arr[index + 1] != null){
        str->data = arr[index];
        str->next = (struct string*) Malloc(sizeof(struct string));
        str = str->next;
        index++;
    }
    str->data = arr[index];
    str->next = (struct string*) Malloc(sizeof(struct string));
    str = str->next;
    str->data = '\0';
}

size_t StringLen(struct string *str){
    size_t index = 0;
    while(str->next != null){
        index++;
        str = str->next;
    }
    return index;
}

uint8_t StringGetAt(struct string *str, size_t pos){
    struct string *temp = str;
    uint8_t val;
    while(pos > 0){
        temp = temp->next;
        pos--;
    }
    val = temp->data;

    return val;
}

void StringAdd(struct string *str, uint8_t ch){
    while(str->next != null){
        str = str->next;
    }
    str->next = (struct string*) Malloc(sizeof(struct string));
    str = str->next;
    str->data = ch;
}

void StringAddFirst(struct string *str, uint8_t ch){
    while(str->next != null){
        str = str->next;
    }

    struct string *temp;
    temp = (struct string*) Malloc(sizeof(struct string));
    temp->data = ch;
    temp->next = null;
    str->next = temp;
}