#pragma once
#include <assert.h>
#include "dmemory.h"

typedef struct dlist{
    void* content;
    struct dlist* next;
} DList;

/* Append data at the end of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
static inline DList* dlist_append(DList* list,void* to_insert);

/* Prepend data at the start of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
static inline DList* dlist_prepend(DList* list,void* to_insert);

/* Free a list but not is content
 */
static inline void dlist_free(DList* list);

/* Search for content match in a list and return the element list containing it or NULL if no match*/
static inline DList* dlist_search(DList* list,void* to_search);








// ---------------------------------------------------
// --------------- IMPLEMENTATION --------------------
// ---------------------------------------------------

static inline DList* dlist_append(DList* list,void* to_insert){

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

static inline DList* dlist_prepend(DList* list,void* to_insert){
    
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


static inline void dlist_free(DList* list){
    
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

/* Search for content match in a list and return the element list containing it or NULL if no match*/
static inline DList* dlist_search(DList* list,void* to_search){
    
    if ( list == NULL)
        return NULL;
    
    if ( list->content == to_search)
        return list;
    
    while(list->next != NULL){
        list = list->next;
        if ( list->content == to_search )
            return list;
    }
    return NULL;
}