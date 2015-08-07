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


static void* memory_pool= 0;
static void* memory_pool_next_available_memory = 0;
static int memory_pool_remaining_memory = 0;

#define POOL_SIZE 1024000

void* mpool_alloc(size_t size){
    if ( memory_pool == 0){
        memory_pool = checked_malloc(POOL_SIZE);
        memory_pool_next_available_memory = memory_pool;
        memory_pool_remaining_memory = POOL_SIZE;
    }
    
    if ( size > memory_pool_remaining_memory){
        printf("Memory pool out of memory");
        exit(EXIT_FAILURE);
    }
    
    void* allocate = memory_pool_next_available_memory;
    memory_pool_remaining_memory -= size;
    memory_pool_next_available_memory += size;
    return allocate;
}

void mpool_release(){
    memory_pool_next_available_memory = memory_pool;
    memory_pool_remaining_memory = POOL_SIZE;    
}

void mpool_free(){
    free(memory_pool);
}