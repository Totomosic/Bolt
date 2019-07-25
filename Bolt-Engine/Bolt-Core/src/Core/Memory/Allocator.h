#pragma once
#include "bltpch.h"

#define BLT_MEM_ALIGN 16
#define BLT_ALLOC(size)	_aligned_malloc(size, BLT_MEM_ALIGN)
#define BLT_FREE(block)	_aligned_free(block);

BLT_API void* Allocate(size_t size);
BLT_API void Free(void* block);

void* operator new(size_t size);
void operator delete(void* block);
void* operator new[](size_t size);
void operator delete[](void* block);