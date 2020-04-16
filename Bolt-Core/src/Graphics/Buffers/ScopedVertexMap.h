#pragma once
#include "Iterators/DefaultVertexIterator.h"
#include "Buffer.h"

namespace Bolt
{

	class VertexBuffer;

	class BLT_API ScopedVertexMap
	{
	private:
		const VertexBuffer* m_Buffer;
		void* m_Ptr;
		size_t m_Offset;
		size_t m_Length;

	public:
		ScopedVertexMap(const VertexBuffer* buffer, Access bufferAccess, size_t offset, size_t length);
		ScopedVertexMap(const ScopedVertexMap& other) = delete;
		ScopedVertexMap& operator=(const ScopedVertexMap& other) = delete;
		ScopedVertexMap(ScopedVertexMap&& other) noexcept;
		ScopedVertexMap& operator=(ScopedVertexMap&& other) noexcept;
		~ScopedVertexMap();

		size_t GetVertexCount() const;
		VertexIterator Begin() const;
		VertexIterator End() const;
		DefaultVertexIterator DefaultBegin() const;
		DefaultVertexIterator DefaultEnd() const;
	};

}