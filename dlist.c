#include <assert.h>

#include "dmemory.h"
#include "dlist.h"

DList* dlist_append(DList* list,void* to_insert){

    assert(to_insert != NULL);
        
    DList* new_element = (DList*)checked_malloc(sizeof(DList));
    new_element->content = to_insert;
    new_element->next = NULL;
    
    if ( list == NULL){
        return new_element;
    }
    
    DList* current = list;
    while(current->next != NULL)
        current = current->next;
    
    current->next = new_element;
    return list;
}

DList* dlist_prepend(DList* list,void* to_insert){
    
    assert(to_insert != NULL);
        
    DList* new_element = (DList*)checked_malloc(sizeof(DList));
    new_element->content = to_insert;
    
    
    if ( list == NULL){
        new_element->next = NULL;
        return new_element;
    }
    
    
    new_element->next = list;
    
    return new_element;
    
}


void dlist_free(DList* list){
    
    if ( list == NULL)
        return;
    
    DList* next_element = list->next;
    free(list);
    
    while ( next_element != NULL){
        list = next_element;
        next_element = list->next;
        free(list);
    }
    
}