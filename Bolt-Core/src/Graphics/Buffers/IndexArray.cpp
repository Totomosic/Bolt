#include "Types.h"
#include "IndexArray.h"
#include "Iterators\__Iterators__.h"

namespace Bolt
{

	IndexArray::IndexArray()
		: m_IndexBuffers(), m_Descriptor(), m_MappedIterators(0)
	{
	
	}

	IndexArray::IndexArray(IndexArray&& other)
		: m_IndexBuffers(std::move(other.m_IndexBuffers)), m_Descriptor(std::move(other.m_Descriptor)), m_MappedIterators(0)
	{
		BLT_ASSERT(!other.IsMapped(), "Unable to move IndexArray while index iterators remain outstanding");
	}

	IndexArray& IndexArray::operator=(IndexArray&& other)
	{
		BLT_ASSERT(!other.IsMapped(), "Unable to move IndexArray while index iterators remain outstanding");
		std::vector<std::unique_ptr<IndexBuffer>> tempIndexBuffers = std::move(m_IndexBuffers);
		m_IndexBuffers = std::move(other.m_IndexBuffers);
		m_Descriptor = other.m_Descriptor;
		m_MappedIterators = 0;
		other.m_IndexBuffers = std::move(tempIndexBuffers);
		return *this;
	}

	const std::vector<std::unique_ptr<IndexBuffer>>& IndexArray::GetIndexBuffers() const
	{
		return m_IndexBuffers;
	}

	int IndexArray::IndexBufferCount() const
	{
		return m_IndexBuffers.size();
	}

	int IndexArray::IndexCount() const
	{
		return m_Descriptor.IndexCount();
	}

	const IArrayDescriptor& IndexArray::Descriptor() const
	{
		return m_Descriptor;
	}

	const std::unique_ptr<IndexBuffer>& IndexArray::GetIndexBuffer(int index) const
	{
		return m_IndexBuffers[index];
	}

	std::unique_ptr<IndexBuffer>& IndexArray::GetIndexBuffer(int index)
	{
		return m_IndexBuffers[index];
	}

	const std::unique_ptr<IndexBuffer>& IndexArray::operator[](int index) const
	{
		return GetIndexBuffer(index);
	}

	std::unique_ptr<IndexBuffer>& IndexArray::operator[](int index)
	{
		return GetIndexBuffer(index);
	}

	bool IndexArray::IsMapped() const
	{
		return m_MappedIterators != 0;
	}

	IndexBuffer& IndexArray::AddIndexBuffer(std::unique_ptr<IndexBuffer>&& buffer)
	{
		IndexBuffer* ptr = buffer.get();
		m_Descriptor.AddIndexBuffer(ptr);
		m_IndexBuffers.push_back(std::move(buffer));
		return *ptr;
	}

	IndexIterator<uint> IndexArray::GetIterator(int index) const
	{
		IArrayDescriptor::QueryResult currentResult = m_Descriptor.QueryIndex(index);
		if (currentResult.Buffer != nullptr)
		{
			m_MappedIterators++;
			return IndexIterator<uint>((byte*)currentResult.Buffer->PrivateMap(Access::ReadWrite) + currentResult.IndexOffset * sizeof(uint), this, index);
		}
		return IndexIterator<uint>((byte*)nullptr, this, index);
	}

	IndexIterator<uint> IndexArray::Begin() const
	{
		return GetIterator(0);
	}

	IndexIterator<uint> IndexArray::End() const
	{
		return GetIterator(m_Descriptor.IndexCount());
	}

	IndexArray IndexArray::Clone() const
	{
		IndexArray result;
		for (const auto& buffer : m_IndexBuffers)
		{
			result.AddIndexBuffer(buffer->Clone());
		}
		return std::move(result);
	}

	byte* IndexArray::TestBufferPointer(int& currentIndex, int prevIndex, byte* currentPtr, int incAmount) const
	{
		IArrayDescriptor::QueryResult currentResult = m_Descriptor.QueryIndex(currentIndex);
		IndexBuffer* currentBuffer = currentResult.Buffer;
		IndexBuffer* prevBuffer = m_Descriptor.GetIndexBuffer(prevIndex);
		byte* ptr = nullptr;
		if (prevBuffer != currentBuffer)
		{
			if (prevBuffer != nullptr && currentBuffer != nullptr)
			{
				prevBuffer->FreeIterator();
			}
			if (currentBuffer != nullptr)
			{
				currentBuffer->MapIterator();
				ptr = (byte*)currentBuffer->PrivateMap(Access::ReadWrite) + currentResult.IndexOffset * sizeof(uint);
			}		
			else
			{
				ptr = currentPtr - sizeof(uint) * incAmount;
				currentIndex -= incAmount;
			}
		}
		else
		{
			ptr = currentPtr + (incAmount) * sizeof(uint);
		}
		return ptr;
	}

}