#include "Types.h"

#include "VertexBuffer.h"

namespace Bolt
{

	VertexBuffer::VertexBuffer(size_t size, BufferLayout layout, BufferUsage usage) : VertexBuffer(nullptr, size, layout, usage)
	{
	
	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, BufferLayout layout, BufferUsage usage) : Buffer(data, size, BufferTarget::ArrayBuffer, usage),
		m_Layout(layout)
	{
		BLT_ASSERT(Size() % layout.Stride() == 0, "Buffer could not fit integer number of vertices");
	}

	VertexBuffer::VertexBuffer() : VertexBuffer(0, BufferLayout())
	{

	}

	const BufferLayout& VertexBuffer::Layout() const
	{
		return m_Layout;
	}

	int VertexBuffer::VertexCount() const
	{
		return Size() / m_Layout.Stride();
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Clone() const
	{
		std::unique_ptr<VertexBuffer> buffer = std::make_unique<VertexBuffer>(Size(), Layout(), Usage());
		byte* data = new byte[Size()];
		Download(data, Size(), 0);
		buffer->Upload(data, Size(), 0);
		delete[] data;
		return std::move(buffer);
	}

}