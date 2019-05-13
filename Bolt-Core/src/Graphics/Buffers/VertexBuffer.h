#pragma once
#include "Buffer.h"
#include "BufferLayout.h"

namespace Bolt
{

	class BLT_API VertexBuffer : public Buffer
	{
	protected:
		BufferLayout m_Layout;
		
	public:
		VertexBuffer(uint32_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(const void* data, uint32_t size, const BufferLayout& layout, BufferUsage usage = BufferUsage::StaticDraw);

		const BufferLayout& Layout() const;
		int VertexCount() const;

		std::unique_ptr<VertexBuffer> Clone() const;

		friend class VertexArray;

	};

}