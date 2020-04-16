#pragma once
#include "Buffer.h"
#include "BufferLayout.h"
#include "ScopedVertexMap.h"

namespace Bolt
{

	class BLT_API VertexBuffer : public Buffer
	{
	protected:
		BufferLayout m_Layout;
		
	public:
		VertexBuffer(size_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(const void* data, size_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);

		const BufferLayout& Layout() const;
		size_t VertexCount() const;
		std::unique_ptr<VertexBuffer> Clone() const;

		ScopedVertexMap MapScoped(Access bufferAccess) const;
		ScopedVertexMap MapScopedRange(size_t startVertexIndex, size_t vertexCount, Access bufferAccess) const;
	};

}