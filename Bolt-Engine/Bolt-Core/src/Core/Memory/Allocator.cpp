#include "bltpch.h"

#include "Allocator.h"

namespace Bolt
{

	std::atomic<size_t> CustomAllocator::s_AllocatedBytes = 0;

	void* CustomAllocator::Allocate(size_t nbytes)
	{
		if (nbytes > 1024 * 1024 * 10)
		{
			BLT_CORE_WARN("Large Allocation: {0} MB", nbytes / 1024.0f / 1024.0f);
		}
#ifndef BLT_DIST
		size_t actualSize = nbytes + sizeof(size_t);
#else
		size_t acutalSize = nbytes;
#endif
		size_t* ptr = (size_t*)BLT_ALLOC(actualSize);
		BLT_ASSERT(ptr != nullptr, "Unable to allocate memory.");
#ifndef BLT_DIST
		*ptr++ = actualSize;
		s_AllocatedBytes += actualSize;
#endif		
		return (void*)ptr;
	}

	void CustomAllocator::Free(void* block)
	{
#ifndef BLT_DIST
		if (block != nullptr)
		{
			size_t* ptr = (size_t*)block - 1;
			size_t allocBytes = *ptr;
			BLT_ASSERT(s_AllocatedBytes >= allocBytes, "Invalid Free");
			s_AllocatedBytes -= allocBytes;
			block = (void*)ptr;
		}
#endif
		return BLT_FREE(block);
	}

	void* CustomAllocator::AllocateDebug(size_t nbytes, const char* file, int line)
	{
#ifndef BLT_DIST
		if (nbytes > 1024 * 1024 * 10)
		{
			BLT_CORE_WARN("Large Allocation: {0} MB\nIn File {1}\n Line: {2}", nbytes / 1024.0f / 1024.0f, file, line);
		}
		size_t actualSize = nbytes + sizeof(size_t);
		size_t* ptr = (size_t*)BLT_ALLOC(actualSize);
		BLT_ASSERT(ptr != nullptr, "Unable to allocate memory.");
		*ptr++ = nbytes;
		s_AllocatedBytes += actualSize;
		return (void*)ptr;
#else
		return Allocate(nbytes);
#endif
	}

	void CustomAllocator::FreeDebug(void* block)
	{
#ifndef BLT_DIST
		if (block != nullptr)
		{
			size_t* ptr = (size_t*)block - 1;
			size_t allocBytes = *ptr;
			BLT_ASSERT(s_AllocatedBytes >= allocBytes, "Invalid Free");
			s_AllocatedBytes -= allocBytes;
			block = (void*)ptr;
		}
		return BLT_FREE(block);
#else
		return Free(block);
#endif
	}

	size_t CustomAllocator::GetAllocatedBytes()
	{
		return s_AllocatedBytes;
	}

}

void* operator new(size_t size)
{
	return Bolt::CustomAllocator::Allocate(size);
}

void operator delete(void* block)
{
	return Bolt::CustomAllocator::Free(block);
}

void* operator new[](size_t size)
{
	return Bolt::CustomAllocator::Allocate(size);
}

void operator delete[](void* block)
{
	return Bolt::CustomAllocator::Free(block);
}

void* operator new(size_t size, const char* file, int line)
{
	return Bolt::CustomAllocator::AllocateDebug(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
	return Bolt::CustomAllocator::AllocateDebug(size, file, line);
}