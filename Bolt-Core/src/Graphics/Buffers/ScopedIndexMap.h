#pragma once
#include "Iterators/IndexIterator.h"
#include "Buffer.h"

namespace Bolt
{
	
	class IndexBuffer;

	class BLT_API ScopedIndexMap
	{
	private:
		const IndexBuffer* m_Buffer;
		void* m_Ptr;
		size_t m_Offset;
		size_t m_Length;

	public:
		ScopedIndexMap(const IndexBuffer* buffer, Access bufferAccess, size_t offset, size_t length);
		ScopedIndexMap(const ScopedIndexMap& other) = delete;
		ScopedIndexMap& operator=(const ScopedIndexMap& other) = delete;
		ScopedIndexMap(ScopedIndexMap&& other) noexcept;
		ScopedIndexMap& operator=(ScopedIndexMap&& other) noexcept;
		~ScopedIndexMap();

		size_t GetIndexCount() const;

		template<typename T>
		IndexIterator<T> Begin() const
		{
			return IndexIterator<T>((T*)m_Ptr);
		}

		template<typename T>
		IndexIterator<T> End() const
		{
			return IndexIterator<T>((T*)((uint8_t*)m_Ptr + m_Length));
		}
	};

}