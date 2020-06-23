#include "mem_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

memory_struct* setup_memory() {
    memory_struct* mem = malloc(sizeof(struct MEM_STRUCT));

    mem->memory_left_over = 0;
    mem->index = 0;
    /* Null pointer, this is assigned as the project goes on.. */
    mem->total_allocated_memory = malloc(1*sizeof(size_t));
    mem->next = calloc(2,sizeof(*mem->next));
    mem->next[0] = *mem;
    mem->next[1] = mem->next[0];

    return mem;
}
memory_struct* Brew_Update_Memory(memory_struct* mem,size_t sizeof_) {
    
    /* I may delete this. This is just for testing */
    mem->next = realloc(mem->next,2*sizeof(*mem->next)+mem->index);

    /* Setting up next[1] */
    mem->next[1] = mem->next[0];

    mem->index++;
    mem->total_allocated_memory = realloc(
        mem->total_allocated_memory,
        (1+mem->index)*sizeof_
    );
    /* Reseting next[0] so we don't run into memory issues ourselves */
    mem->next[0].allocated.allocated_size = 0;
    mem->next[0].reallocated.re_allocated_size = 0;
    mem->next[0].calloc_.calloc_index_size = 0;
    mem->next[0].calloc_.allocated_index_size = 0;
    mem->next[0] = *mem;

    return mem;
}

char* Brew_Realloc_Memory_Strict(void* ptr, size_t mem_to_assign, size_t sizeof_,memory_struct* mem) {
    size_t total_size = 0;

    ptr = realloc(
        ptr,
        mem_to_assign*sizeof_
    );
    mem->reallocated.re_allocated_size = mem_to_assign*sizeof_;
    mem->total_allocated_memory[mem->index] = mem->reallocated.re_allocated_size;
    Brew_Update_Memory(mem, sizeof(mem->reallocated.re_allocated_size));

    /* Checking memory allocation */
    for(int i = 0; i < strlen(ptr); i++) {
        total_size+=8;
    }
    if(mem_to_assign*sizeof_>total_size/8) {
        printf("\n\nErr: Memory reallocation failed.\nAllocated %ld bits less than what was aquired.\n\n",mem_to_assign*sizeof_-total_size/8);
        exit(1);
    }
    if(mem_to_assign*sizeof_<total_size/8) {
        printf("\n\nErr: Memory reallocation failed.\nAllocated %ld bits above what was aquired.\n\n",total_size/8-mem_to_assign*sizeof_);
        exit(1);
    }

    return ptr;
}
void* Brew_Allocate_Memory(void *ptr, size_t mem_to_assign, size_t sizeof_,memory_struct* mem) {
    ptr = malloc(
        mem_to_assign*sizeof_
    );
    mem->allocated.allocated_size = mem_to_assign*sizeof_;
    mem->total_allocated_memory[mem->index] = mem->allocated.allocated_size;

    /* Updating memory block */
    mem = Brew_Update_Memory(mem,sizeof(mem->total_allocated_memory));

    return ptr;
}