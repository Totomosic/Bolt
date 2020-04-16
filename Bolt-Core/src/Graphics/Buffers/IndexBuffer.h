#pragma once
#include "Buffer.h"
#include "ScopedIndexMap.h"

namespace Bolt
{

	class BLT_API IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(uint32_t indexCount, BufferUsage usage = BufferUsage::StaticDraw);
		IndexBuffer(const uint32_t* indices, uint32_t indexCount, BufferUsage usage = BufferUsage::StaticDraw);

		constexpr GLenum IndexType() const { return GL_UNSIGNED_INT; }
		int IndexCount() const;
		std::unique_ptr<IndexBuffer> Clone() const;

		ScopedIndexMap MapScoped(Access bufferAccess) const;
		ScopedIndexMap MapScopedRange(size_t startIndex, size_t indexCount, Access bufferAccess) const;
	};

}