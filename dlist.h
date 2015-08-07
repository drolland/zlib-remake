#pragma once

typedef struct dlist{
    void* content;
    struct dlist* next;
} DList;

/* Append data at the end of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
DList* dlist_append(DList* list,void* to_insert);



/* Free a list but not is content
 */
void dlist_free(DList* list);

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