#include<stdio.h>

#ifndef LIST_H
#define LIST_H

typedef struct dlk_element_t{
    void* data;
    struct dlk_element_t* prev;
    struct dlk_element_t* next;
}dlk_element_t;

typedef struct dlk_list_t{
    dlk_element_t* head;
    dlk_element_t* tail;
}dlk_list_t;

void dlk_list_init(dlk_list_t* list);
void dlk_list_append(dlk_list_t* list, dlk_element_t* element);
void dlk_list_remove(dlk_list_t* list, dlk_element_t* element);
void dlk_list_move_element_to(dlk_list_t* src, dlk_list_t* dst, dlk_element_t* element);

#endif
