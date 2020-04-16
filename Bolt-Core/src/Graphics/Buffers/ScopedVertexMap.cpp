#include "bltpch.h"
#include "ScopedVertexMap.h"
#include "VertexBuffer.h"

namespace Bolt
{

	ScopedVertexMap::ScopedVertexMap(const VertexBuffer* buffer, Access bufferAccess, size_t offset, size_t length)
		: m_Buffer(buffer), m_Ptr(nullptr), m_Offset(offset), m_Length(length)
	{
		m_Ptr = m_Buffer->MapRange(m_Offset, m_Length, bufferAccess);
	}

	ScopedVertexMap::ScopedVertexMap(ScopedVertexMap&& other) noexcept
		: m_Buffer(other.m_Buffer), m_Ptr(other.m_Ptr), m_Offset(other.m_Offset), m_Length(other.m_Length)
	{
		other.m_Buffer = nullptr;
		other.m_Ptr = nullptr;
	}

	ScopedVertexMap& ScopedVertexMap::operator=(ScopedVertexMap&& other) noexcept
	{
		const VertexBuffer* buffer = m_Buffer;
		void* ptr = m_Ptr;
		size_t offset = m_Offset;
		size_t length = m_Length;
		m_Buffer = other.m_Buffer;
		m_Ptr = other.m_Ptr;
		m_Offset = other.m_Offset;
		m_Length = other.m_Length;
		other.m_Buffer = buffer;
		other.m_Ptr = ptr;
		other.m_Offset = offset;
		other.m_Length = length;
		return (*this);
	}

	ScopedVertexMap::~ScopedVertexMap()
	{
		if (m_Buffer && m_Ptr && m_Buffer->IsMapped())
		{
			m_Buffer->Unmap();
		}
	}

	size_t ScopedVertexMap::GetVertexCount() const
	{
		return m_Length / m_Buffer->Layout().Size();
	}

	VertexIterator ScopedVertexMap::Begin() const
	{
		return VertexIterator(m_Ptr, &m_Buffer->Layout());
	}

	VertexIterator ScopedVertexMap::End() const
	{
		return VertexIterator((uint8_t*)m_Ptr + m_Length, &m_Buffer->Layout());
	}

	DefaultVertexIterator ScopedVertexMap::DefaultBegin() const
	{
		return DefaultVertexIterator(m_Ptr, &m_Buffer->Layout());
	}

	DefaultVertexIterator ScopedVertexMap::DefaultEnd() const
	{
		return DefaultVertexIterator((uint8_t*)m_Ptr + m_Length, &m_Buffer->Layout());
	}

}
