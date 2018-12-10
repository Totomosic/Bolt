#include "Types.h"
#include "VertexIterator.h"
#include "..\VertexArray.h"

namespace Bolt
{

	VertexIterator::VertexIterator()
		: m_VertexBuffers(), m_VertexIndex(-1)
	{
	
	}
	
	VertexIterator::VertexIterator(MappedVertexArray&& vertexBuffers, int vertexIndex)
		: m_VertexBuffers(std::move(vertexBuffers)), m_VertexIndex(vertexIndex)
	{
	
	}

	VertexIterator::VertexIterator(VertexIterator&& other)
		: m_VertexBuffers(std::move(other.m_VertexBuffers)), m_VertexIndex(other.m_VertexIndex)
	{
		
	}

	VertexIterator& VertexIterator::operator=(VertexIterator&& other)
	{
		m_VertexBuffers = std::move(other.m_VertexBuffers);
		m_VertexIndex = other.m_VertexIndex;
		return *this;
	}

	VertexIterator::~VertexIterator() noexcept
	{
		
	}

	int VertexIterator::VertexIndex() const
	{
		return m_VertexIndex;
	}

	AttributeSetter VertexIterator::operator[](int attributeIndex)
	{
		return Seek(attributeIndex);
	}

	AttributeSetter VertexIterator::Seek(int attributeIndex)
	{
		MappedVertexArray::MappedPtr& mappedPtr = m_VertexBuffers.m_MappedPtrs.at(attributeIndex);
		byte* ptr = mappedPtr.Ptr;
		ptr += mappedPtr.Layout->OffsetOf(attributeIndex);
		ptr += m_VertexIndex * mappedPtr.Layout->Stride();
		return AttributeSetter(ptr, attributeIndex);
	}

	VertexIterator& VertexIterator::operator++()
	{
		m_VertexIndex++;
		return *this;
	}

	VertexIterator& VertexIterator::operator--()
	{
		m_VertexIndex--;
		return *this;
	}

	VertexIterator& VertexIterator::operator++(int)
	{
		m_VertexIndex++;
		return *this;
	}

	VertexIterator& VertexIterator::operator--(int)
	{
		m_VertexIndex--;
		return *this;
	}

	VertexIterator& VertexIterator::operator+=(int amount)
	{
		m_VertexIndex += amount;
		return *this;
	}

	VertexIterator& VertexIterator::operator-=(int amount)
	{
		m_VertexIndex -= amount;
		return *this;
	}

	bool VertexIterator::operator==(const VertexIterator& other) const
	{
		return (m_VertexIndex == other.m_VertexIndex);
	}

	bool VertexIterator::operator!=(const VertexIterator& other) const
	{
		return !(*this == other);
	}

	bool VertexIterator::operator<(const VertexIterator& other) const
	{
		return (m_VertexIndex < other.m_VertexIndex);
	}

	bool VertexIterator::operator<=(const VertexIterator& other) const
	{
		return (m_VertexIndex <= other.m_VertexIndex);
	}

	bool VertexIterator::operator>(const VertexIterator& other) const
	{
		return (m_VertexIndex > other.m_VertexIndex);
	}

	bool VertexIterator::operator>=(const VertexIterator& other) const
	{
		return (m_VertexIndex >= other.m_VertexIndex);
	}

}