#pragma once
#include "Buffer.h"

namespace Bolt
{

	class BLT_API IndexBuffer : public Buffer
	{
	private:
		mutable int m_MappedIterators;

	public:
		IndexBuffer(size_t indexCount, BufferUsage usage = BufferUsage::StaticDraw);
		IndexBuffer(const uint* indices, size_t indexCount, BufferUsage usage = BufferUsage::StaticDraw);

		constexpr GLenum IndexType() const { return GL_UNSIGNED_INT; }
		int IndexCount() const;

		friend class IndexArray;

	private:
		void MapIterator() const;
		void FreeIterator() const;

		void* PrivateMap(Access access) const;
		bool PrivateUnmap() const;

	};

}