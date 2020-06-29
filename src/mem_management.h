#ifndef MEM_H
#define MEM_H
#include <stdlib.h>

typedef struct MEM_STRUCT {
    int index;
    struct {
        size_t re_allocated_size;
    } reallocated;
    struct {
        size_t allocated_size;
    } allocated;
    struct {
        int calloc_index_size;
        size_t allocated_index_size;
    } calloc_;
    struct {
        size_t DeAllocatedSize;
    } DeAllocate;
    size_t* total_allocated_memory;
    size_t memory_left_over;
    struct MEM_STRUCT* next;
} memory_struct;

memory_struct* setup_memory();

char* Brew_Strict_DeAllocate(void* ptr, size_t strict_size, size_t size_of_allocation, size_t auto_allocate, memory_struct* mem);
/* Stricly reallocates memory depending on ptr */
char* Brew_Realloc_Memory_Strict(void* ptr, size_t mem_to_assign, size_t sizeof_,memory_struct* mem);
/* Lazily reallocates memory. Doesn't care if it is above or below what the ptr needs. */
char* Brew_Realloc_Memory_Lazy(void* ptr, size_t mem_to_assign, size_t sizeof_,memory_struct* mem);
void* Brew_Allocate_Memory(void *ptr, size_t mem_to_assign, size_t sizeof_,memory_struct* mem);

#endif