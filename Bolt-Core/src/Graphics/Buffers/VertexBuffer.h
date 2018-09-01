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
		VertexBuffer(size_t size, BufferLayout layout, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(const void* data, size_t size, BufferLayout layout, BufferUsage usage = BufferUsage::StaticDraw);

		const BufferLayout& Layout() const;
		int VertexCount() const;

		std::unique_ptr<VertexBuffer> Clone() const;

		friend class VertexArray;
		friend class MappedBufferManager;
		friend class IteratorManager;
		friend class std::vector<VertexBuffer>;
		friend class std::unique_ptr<VertexBuffer>;

	private:
		VertexBuffer();

	};

}