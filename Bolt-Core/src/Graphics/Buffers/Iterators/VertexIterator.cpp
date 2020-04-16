#include "bltpch.h"
#include "VertexIterator.h"

namespace Bolt
{

	VertexIterator::VertexIterator() : VertexIterator(nullptr, {})
	{
	
	}
	
	VertexIterator::VertexIterator(void* buffer, const BufferLayout* layout)
		: m_Buffer(buffer), m_Layout(layout)
	{
		
	}

	AttributeSetter VertexIterator::operator[](int attributeIndex) const
	{
		return GetSetter(attributeIndex);
	}

	AttributeSetter VertexIterator::GetSetter(int attributeIndex) const
	{
		return AttributeSetter((uint8_t*)m_Buffer + m_Layout->OffsetOf(attributeIndex), attributeIndex);
	}

	VertexIterator& VertexIterator::operator++()
	{
		(*this) += 1;
		return *this;
	}

	VertexIterator& VertexIterator::operator--()
	{
		(*this) -= 1;
		return *this;
	}

	VertexIterator VertexIterator::operator++(int)
	{
		(*this) += 1;
		return (*this - 1);
	}

	VertexIterator VertexIterator::operator--(int)
	{
		(*this) -= 1;
		return (*this + 1);
	}

	VertexIterator& VertexIterator::operator+=(int amount)
	{
		m_Buffer = (void*)((uint8_t*)m_Buffer + m_Layout->Size() * amount);
		return *this;
	}

	VertexIterator& VertexIterator::operator-=(int amount)
	{
		m_Buffer = (void*)((uint8_t*)m_Buffer - m_Layout->Size() * amount);
		return *this;
	}

	VertexIterator operator+(const VertexIterator& left, int right)
	{
		return VertexIterator((uint8_t*)left.m_Buffer + left.m_Layout->Size() * right, left.m_Layout);
	}

	VertexIterator operator-(const VertexIterator& left, int right)
	{
		return (left + (-right));
	}

	bool VertexIterator::operator==(const VertexIterator& other) const
	{
		return (m_Buffer == other.m_Buffer);
	}

	bool VertexIterator::operator!=(const VertexIterator& other) const
	{
		return !(*this == other);
	}

	bool VertexIterator::operator<(const VertexIterator& other) const
	{
		return (m_Buffer < other.m_Buffer);
	}

	bool VertexIterator::operator<=(const VertexIterator& other) const
	{
		return (m_Buffer <= other.m_Buffer);
	}

	bool VertexIterator::operator>(const VertexIterator& other) const
	{
		return (m_Buffer > other.m_Buffer);
	}

	bool VertexIterator::operator>=(const VertexIterator& other) const
	{
		return (m_Buffer >= other.m_Buffer);
	}

}