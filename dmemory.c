#include <stdio.h>
#include "dmemory.h"

void* checked_malloc(size_t size){
    void* mem = malloc(size);
    if ( mem == NULL){
        printf("Memory Allocation failure, terminating...");
        exit(EXIT_FAILURE);
    }
    return mem;
}
