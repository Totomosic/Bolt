#include "bltpch.h"
#include "ScopedIndexMap.h"
#include "IndexBuffer.h"

namespace Bolt
{

	ScopedIndexMap::ScopedIndexMap(const IndexBuffer* buffer, Access bufferAccess, size_t offset, size_t length)
		: m_Buffer(buffer), m_Ptr(nullptr), m_Offset(offset), m_Length(length)
	{
		m_Ptr = m_Buffer->MapRange(m_Offset, m_Length, bufferAccess);
	}

	ScopedIndexMap::ScopedIndexMap(ScopedIndexMap&& other) noexcept
		: m_Buffer(other.m_Buffer), m_Ptr(other.m_Ptr), m_Offset(other.m_Offset), m_Length(other.m_Length)
	{
		other.m_Buffer = nullptr;
		other.m_Ptr = nullptr;
	}

	ScopedIndexMap& ScopedIndexMap::operator=(ScopedIndexMap&& other) noexcept
	{
		const IndexBuffer* buffer = m_Buffer;
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

	ScopedIndexMap::~ScopedIndexMap()
	{
		if (m_Buffer && m_Ptr && m_Buffer->IsMapped())
		{
			m_Buffer->Unmap();
		}
	}

	size_t ScopedIndexMap::GetIndexCount() const
	{
		return m_Length / sizeof(uint32_t);
	}

}
