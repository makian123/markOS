#include "lib/string.h"
#include "lib/terminal.h"

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
    while(str->next->next != null){
        str = str->next;
    }
    str->next = (struct string*) Malloc(sizeof(struct string));
    str->next->data = ch;
    str->next->next = (struct string*) Malloc(sizeof(struct string));
}

void StringAddFirst(struct string *str, uint8_t ch){
    struct string *temp = (struct string*) Malloc(sizeof(struct string));
    temp->data = ch;
    temp->next = str;
    str = temp;
}

size_t StringFind(struct string *str, uint8_t ch){
    size_t index = 0;
    while(str->next != null){
        if(str->data == ch) break;
        index++;
        str = str->next;
    }
    return index;
}

void StringChangeAt(struct string *str, uint8_t ch, size_t pos){
    while(pos > 0 && str->next != null){
        str = str->next;
        pos--;
    }
    if(pos == 0){
        str->data = ch;
    }
}

void PopBack(struct string *str){
    while(str->next->next != null){
        str = str->next;
    }

    Free(str->next);
}