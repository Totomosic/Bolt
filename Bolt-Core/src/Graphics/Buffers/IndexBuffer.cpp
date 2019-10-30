#include "bltpch.h"

#include "IndexBuffer.h"

namespace Bolt
{

	IndexBuffer::IndexBuffer(uint32_t indexCount, BufferUsage usage) : IndexBuffer(nullptr, indexCount, usage)
	{

	}

	IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t indexCount, BufferUsage usage) : Buffer((const void*)indices, indexCount * sizeof(uint32_t), BufferTarget::ElementBuffer, usage)
	{

	}

	int IndexBuffer::IndexCount() const
	{
		return Size() / sizeof(uint32_t);
	}

	std::unique_ptr<IndexBuffer> IndexBuffer::Clone() const
	{
		std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(Size() / sizeof(uint32_t), Usage());
		byte* data = BLT_NEW byte[Size()];
		Download(data, Size(), 0);
		indexBuffer->Upload(data, Size(), 0);
		BLT_DELETE_ARR data;
		return indexBuffer;
	}

	void IndexBuffer::MapIterator() const
	{
		m_MappedIterators++;
	}

	void IndexBuffer::FreeIterator() const
	{
		m_MappedIterators--;
		if (m_MappedIterators <= 0)
		{
			Buffer::Unmap();
			m_MappedIterators = 0;
		}
	}

	void* IndexBuffer::PrivateMap(Access access) const
	{
		return Buffer::Map(access);
	}

	bool IndexBuffer::PrivateUnmap() const
	{
		return Buffer::Unmap();
	}

}