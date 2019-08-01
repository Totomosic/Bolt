#include "bltpch.h"

#include "Allocator.h"

void* Allocate(size_t size)
{
	if (size > 1024 * 1024 * 10)
	{
		BLT_CORE_WARN("Large Allocation: {} MB", size / 1024.0f / 1024.0f);
	}
	void* ptr = BLT_ALLOC(size);
	BLT_ASSERT(ptr != nullptr, "Unable to allocate memory.");
	return ptr;
}

void Free(void* block)
{
	return BLT_FREE(block);
}

void* operator new(size_t size)
{
	return Allocate(size);
}

void operator delete(void* block)
{
	return Free(block);
}

void* operator new[](size_t size)
{
	return Allocate(size);
}

void operator delete[](void* block)
{
	return Free(block);
}