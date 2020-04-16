#include "bltpch.h"

#include "IndexArray.h"
#include "Iterators/IndexIterator.h"

namespace Bolt
{

	IndexArray::IndexArray()
		: m_IndexBuffers(), m_Descriptor()
	{
	
	}

	IndexArray::IndexArray(IndexArray&& other) noexcept
		: m_IndexBuffers(std::move(other.m_IndexBuffers)), m_Descriptor(std::move(other.m_Descriptor))
	{
		BLT_ASSERT(!other.IsMapped(), "Cannot move mapped array");
	}

	IndexArray& IndexArray::operator=(IndexArray&& other) noexcept
	{
		BLT_ASSERT(!other.IsMapped(), "Cannot move mapped array");
		std::vector<std::unique_ptr<IndexBuffer>> tempIndexBuffers = std::move(m_IndexBuffers);
		m_IndexBuffers = std::move(other.m_IndexBuffers);
		m_Descriptor = other.m_Descriptor;
		other.m_IndexBuffers = std::move(tempIndexBuffers);
		return *this;
	}

	IndexArray::~IndexArray()
	{
		BLT_ASSERT(!IsMapped(), "Cannot delete mapped IndexArray");
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
		for (const auto& ptr : m_IndexBuffers)
			if (ptr->IsMapped())
				return true;
		return false;
	}

	IndexBuffer& IndexArray::AddIndexBuffer(std::unique_ptr<IndexBuffer>&& buffer)
	{
		IndexBuffer* ptr = buffer.get();
		m_Descriptor.AddIndexBuffer(ptr);
		m_IndexBuffers.push_back(std::move(buffer));
		return *ptr;
	}

	std::unique_ptr<IndexArray> IndexArray::Clone() const
	{
		std::unique_ptr<IndexArray> result = std::make_unique<IndexArray>();
		for (const auto& buffer : m_IndexBuffers)
		{
			result->AddIndexBuffer(buffer->Clone());
		}
		return result;
	}

}