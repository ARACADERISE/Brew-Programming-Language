#include "mem_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* Brew_Realloc_Memory_Strict(void* ptr, size_t mem_to_assign, size_t sizeof_) {
    size_t total_size = 0;

    ptr = realloc(
        ptr,
        mem_to_assign*sizeof_
    );

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