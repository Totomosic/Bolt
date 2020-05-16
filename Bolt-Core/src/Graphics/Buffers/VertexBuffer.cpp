#include "bltpch.h"
#include "VertexBuffer.h"

namespace Bolt
{

	VertexBuffer::VertexBuffer(size_t size, const BufferLayout& layout, BufferUsage usage) : VertexBuffer(nullptr, size, layout, usage)
	{
	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, const BufferLayout& layout, BufferUsage usage) : Buffer(data, size, BufferTarget::ArrayBuffer, usage),
		m_Layout(layout)
	{
		BLT_ASSERT(Size() % layout.Stride() == 0, "Buffer could not fit integer number of vertices");
	}

	const BufferLayout& VertexBuffer::Layout() const
	{
		return m_Layout;
	}

	size_t VertexBuffer::VertexCount() const
	{
		return Size() / m_Layout.Size();
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Clone() const
	{
		std::unique_ptr<VertexBuffer> buffer = std::make_unique<VertexBuffer>(Size(), Layout(), Usage());
		byte* data = BLT_NEW byte[Size()];
		Download(data, Size(), 0);
		buffer->Upload(data, Size(), 0);
		BLT_DELETE_ARR data;
		return buffer;
	}

	ScopedVertexMap VertexBuffer::MapScoped(Access bufferAccess) const
	{
		return ScopedVertexMap(this, bufferAccess, 0, Size());
	}

	ScopedVertexMap VertexBuffer::MapScopedRange(size_t startVertexIndex, size_t vertexCount, Access bufferAccess) const
	{
		return ScopedVertexMap(this, bufferAccess, startVertexIndex * Layout().Size(), vertexCount * Layout().Size());
	}

}