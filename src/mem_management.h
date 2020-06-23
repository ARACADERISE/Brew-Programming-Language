#ifndef MEM_H
#define MEM_H
#include <stdlib.h>

/* Stricly reallocates memory depending on ptr */
char* Brew_Realloc_Memory_Strict(void* ptr, size_t mem_to_assign, size_t sizeof_);
/* Lazily reallocates memory. Doesn't care if it is above or below what the ptr needs. */
char* Brew_Realloc_Memory_Lazy(void* ptr, size_t mem_to_assign, size_t sizeof_);

#endif