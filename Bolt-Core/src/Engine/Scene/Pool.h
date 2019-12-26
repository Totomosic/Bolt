#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API BasePool
	{
	private:
		std::vector<std::unique_ptr<uint8_t[]>> m_Blocks;
		size_t m_ElementSize;
		size_t m_ChunkSize;
		size_t m_Size;
		size_t m_Capacity;

	public:
		BasePool(size_t elementSize, size_t chunkSize = 8192)
			: m_ElementSize(elementSize), m_ChunkSize(chunkSize), m_Size(0), m_Capacity(0)
		{
		
		}

		virtual ~BasePool() {}

		inline size_t GetSize() const { return m_Size; }
		inline size_t GetCapacity() const { return m_Capacity; }
		inline size_t GetChunks() const { return m_Blocks.size(); }

		// Ensure n elements fit in pool
		inline void Expand(size_t n)
		{
			if (n >= GetSize())
			{
				if (n >= GetCapacity())
				{
					Reserve(n);
				}
				m_Size = n;
			}
		}

		inline void Reserve(size_t n)
		{
			while (GetCapacity() < n)
			{
				std::unique_ptr<uint8_t[]> chunk = std::make_unique<uint8_t[]>(m_ElementSize * m_ChunkSize);
				m_Blocks.push_back(std::move(chunk));
				m_Capacity += m_ChunkSize;
			}
		}

		inline const void* Get(size_t index) const
		{
			BLT_ASSERT(index < GetSize(), "Index out of range");
			return m_Blocks[index / m_ChunkSize].get() + (index % m_ChunkSize) * m_ElementSize;
		}

		inline void* Get(size_t index)
		{
			BLT_ASSERT(index < GetSize(), "Index out of range");
			return m_Blocks[index / m_ChunkSize].get() + (index % m_ChunkSize) * m_ElementSize;
		}

		virtual void Destroy(size_t index) = 0;

	};

	template<typename T, size_t ChunkSize = 8192>
	class BLT_API Pool : public BasePool
	{
	public:
		Pool() : BasePool(sizeof(T), ChunkSize)
		{
		
		}

		inline virtual void Destroy(size_t index) override
		{
			BLT_ASSERT(index < GetSize(), "Index out of range");
			T* ptr = (T*)Get(index);
			ptr->~T();
		}
	};

}