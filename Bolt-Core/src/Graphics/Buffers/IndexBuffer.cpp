#include "IndexBuffer.h"

namespace Bolt
{

	IndexBuffer::IndexBuffer(size_t indexCount, BufferUsage usage) : IndexBuffer(nullptr, indexCount, usage)
	{

	}

	IndexBuffer::IndexBuffer(const uint* indices, size_t indexCount, BufferUsage usage) : Buffer((const void*)indices, indexCount * sizeof(uint), BufferTarget::ElementBuffer, usage)
	{

	}

	int IndexBuffer::IndexCount() const
	{
		return Size() / sizeof(uint);
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