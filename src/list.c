#include"list.h"

void dlk_list_init(dlk_list_t* list){
    list->head = NULL;
    list->tail = NULL;
}

/** Append the element at the end of the list
 *
 */
void dlk_list_append(dlk_list_t* list, dlk_element_t* element){
    element->prev = list->head;
    if(element->prev){
        element->prev->next = element;
    }
    element->next = NULL;
    list->head = element;

    if(!list->tail){
        list->tail = element;
    }
}

/** Unlink the element from the list.
 *
 */
void dlk_list_remove(dlk_list_t* list, dlk_element_t* element){
    if(element == list->head){
        list->head = element->prev;
    }

    if(element == list->tail){
        list->tail = element->next;
    }
    
    if(element->prev){
        element->prev->next = element->next;
    }

    if(element->next){
        element->next->prev = element->prev;
    }
    
    element->prev = NULL;
    element->next = NULL;
}

void dlk_list_move_element_to(dlk_list_t* src, dlk_list_t* dst, dlk_element_t* element){
    dlk_list_remove(src, element);
    dlk_list_append(dst, element);
}
