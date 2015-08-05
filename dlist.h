#pragma once

typedef struct dlist{
    void* content;
    struct dlist* next;
} DList;

DList* dlist_append(DList** list,void* to_insert);

