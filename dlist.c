#include <assert.h>

#include "dmemory.h"
#include "dlist.h"

DList* dlist_append(DList** list,void* to_insert){

    assert(to_insert != NULL);
    
    DList* new_element = (DList*)checked_malloc(sizeof(DList));
    new_element->content = to_insert;
    new_element->next = NULL;
    
    if ( *list == NULL){
        *list = new_element;
        return new_element;
    }
    
    DList* current = *list;
    while(current->next != NULL)
        current = current->next;
    
    current->next = new_element;
    return new_element;
}
