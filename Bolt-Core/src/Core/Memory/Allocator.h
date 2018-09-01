#pragma once
#include "Types.h"

#define BLT_MEM_ALIGN 16
#define BLT_ALLOC(size)	_aligned_malloc(size, BLT_MEM_ALIGN)
#define BLT_FREE(block)	_aligned_free(block);

void* Allocate(size_t size);
void Free(void* block);

void* operator new(size_t size);
void operator delete(void* block);
void* operator new[](size_t size);
void operator delete[](void* block);