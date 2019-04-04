#include "Types.h"
#include "VertexIterator.h"
#include "../VertexMapping.h"

namespace Bolt
{

	VertexIterator::VertexIterator()
		: m_Mapping(nullptr), m_VertexIndex(-1)
	{
	
	}
	
	VertexIterator::VertexIterator(const VertexMapping* mapping, int vertexIndex)
		: m_Mapping(mapping), m_VertexIndex(vertexIndex)
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
		return AttributeSetter(m_Mapping->GetAttributePtr(attributeIndex, m_VertexIndex), attributeIndex);
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