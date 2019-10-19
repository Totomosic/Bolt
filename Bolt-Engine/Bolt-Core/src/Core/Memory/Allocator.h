#pragma once
#include "Logging.h"
#include <atomic>
#include <stdint.h>
#include <memory>

#define BLT_MEM_ALIGN 16
#define BLT_ALLOC(size)	malloc(size)
#define BLT_FREE(block)	free(block)

// Memory
#ifndef BLT_DIST
#define BLT_NEW new((const char*)__FILE__, __LINE__)
#define BLT_DELETE delete
#define BLT_DELETE_ARR delete[]
#else
#define BLT_NEW new
#define BLT_DELETE delete
#define BLT_DELETE_ARR delete[]
#endif

namespace Bolt
{

	class BLT_API CustomAllocator
	{
	private:
		static std::atomic<size_t> s_AllocatedBytes;

	public:
		CustomAllocator() = delete;

		static void* Allocate(size_t size);
		static void Free(void* block);

		static void* AllocateDebug(size_t size, const char* file, int line);
		static void FreeDebug(void* block);

		static size_t GetAllocatedBytes();
	};

}

void* operator new(size_t size);
void operator delete(void* block);
void* operator new[](size_t size);
void operator delete[](void* block);

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);