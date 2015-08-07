#include <assert.h>

#include "dmemory.h"
#include "dlist.h"

/* Append data at the end of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
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

/* Free a list but not is content
 */
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