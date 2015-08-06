#pragma once

typedef struct dlist{
    void* content;
    struct dlist* next;
} DList;

/* Append data at the end of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
DList* dlist_append(DList* list,void* to_insert);

/* Insert an data after provided list element
 * Return the new element
 */
DList* dlist_insert_after(DList* element,void* to_insert);

/* Free a list but not is content
 */
void dlist_free(DList* list);