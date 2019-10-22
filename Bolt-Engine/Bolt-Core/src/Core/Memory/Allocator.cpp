#include "bltpch.h"

#include "Allocator.h"

#ifndef BLT_DIST
#define BLT_DEBUG_SWITCH(debug, ndebug) debug
#else
#define BLT_DEBUG_SWITCH(debugm ndebug) ndebug
#endif

namespace Bolt
{

	std::atomic<size_t> CustomAllocator::s_AllocatedBytes = 0;
	std::atomic<size_t> CustomAllocator::s_AllocationCount = 0;

	void* CustomAllocator::Allocate(size_t nbytes)
	{
		if (nbytes > 1024 * 1024 * 10)
		{
			BLT_CORE_WARN("Large Allocation: {0} MB", nbytes / 1024.0f / 1024.0f);
		}
		size_t* ptr = (size_t*)BLT_ALLOC(nbytes);
		BLT_ASSERT(ptr != nullptr, "Unable to allocate memory.");
		return (void*)ptr;
	}

	void CustomAllocator::Free(void* block)
	{
		return BLT_FREE(block);
	}

	void* CustomAllocator::AllocateDebug(size_t nbytes, const char* file, int line)
	{
		if (nbytes > 1024 * 1024 * 10)
		{
			BLT_CORE_WARN("Large Allocation: {0} MB\nIn File {1}\n Line: {2}", nbytes / 1024.0f / 1024.0f, file, line);
		}
		size_t actualSize = nbytes + sizeof(size_t);
		size_t* ptr = (size_t*)BLT_ALLOC(actualSize);
		BLT_ASSERT(ptr != nullptr, "Unable to allocate memory.");
		*ptr++ = actualSize;
		s_AllocatedBytes += actualSize;
		s_AllocationCount++;
		return (void*)ptr;
	}

	void CustomAllocator::FreeDebug(void* block)
	{
		if (block != nullptr)
		{
			size_t* ptr = (size_t*)block - 1;
			size_t allocBytes = *ptr;
			BLT_ASSERT(s_AllocatedBytes >= allocBytes, "Invalid Free");
			s_AllocatedBytes -= allocBytes;
			s_AllocationCount--;
			block = (void*)ptr;
		}
		return BLT_FREE(block);
	}

	size_t CustomAllocator::GetAllocatedBytes()
	{
		return s_AllocatedBytes;
	}

	size_t CustomAllocator::GetAllocationCount()
	{
		return s_AllocationCount;
	}

}

void* operator new(size_t size)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::AllocateDebug(size, "Unknown", 0), Bolt::CustomAllocator::Allocate(size));
}

void operator delete(void* block)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::FreeDebug(block), Bolt::CustomAllocator::Free(block));
}

void* operator new[](size_t size)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::AllocateDebug(size, "Unknown", 0), Bolt::CustomAllocator::Allocate(size));
}

void operator delete[](void* block)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::FreeDebug(block), Bolt::CustomAllocator::Free(block));
}

void* operator new(size_t size, const char* file, int line)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::AllocateDebug(size, file, line), Bolt::CustomAllocator::Allocate(size));
}

void* operator new[](size_t size, const char* file, int line)
{
	return BLT_DEBUG_SWITCH(Bolt::CustomAllocator::AllocateDebug(size, file, line), Bolt::CustomAllocator::Allocate(size));
}