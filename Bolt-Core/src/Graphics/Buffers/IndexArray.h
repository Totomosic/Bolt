#pragma once
#include "IndexBuffer.h"
#include "IArrayDescriptor.h"

namespace Bolt
{

	template<typename> class IndexIterator;

	class BLT_API IndexArray
	{
	private:
		std::vector<std::unique_ptr<IndexBuffer>> m_IndexBuffers;
		IArrayDescriptor m_Descriptor;
		mutable int m_MappedIterators;

	public:
		IndexArray();
		IndexArray(const IndexArray& other) = delete;
		IndexArray& operator=(const IndexArray& other) = delete;
		IndexArray(IndexArray&& other);
		IndexArray& operator=(IndexArray&& other);
		~IndexArray() = default;

		const std::vector<std::unique_ptr<IndexBuffer>>& GetIndexBuffers() const;
		int IndexBufferCount() const;
		int IndexCount() const;
		const IArrayDescriptor& Descriptor() const;
		const std::unique_ptr<IndexBuffer>& GetIndexBuffer(int index) const;
		std::unique_ptr<IndexBuffer>& GetIndexBuffer(int index);
		const std::unique_ptr<IndexBuffer>& operator[](int index) const;
		std::unique_ptr<IndexBuffer>& operator[](int index);
		bool IsMapped() const;

		IndexBuffer& AddIndexBuffer(std::unique_ptr<IndexBuffer>&& buffer);

		IndexIterator<uint> GetIterator(int index) const;
		IndexIterator<uint> Begin() const;
		IndexIterator<uint> End() const;

		IndexArray Clone() const;

		template<typename> friend class IndexIterator;

	private:
		byte* TestBufferPointer(int& currentIndex, int prevIndex, byte* currentPtr, int incAmount) const;
		
		template<typename T>
		void FreeIterator(IndexIterator<T>& iterator) const
		{
			IndexBuffer* buffer = m_Descriptor.GetIndexBuffer(iterator.m_CurrentIndex);
			buffer->PrivateUnmap();
			m_MappedIterators--;
		}

	};

}