#include "Types.h"
#include "IndexIterator.h"
#include "../IndexMapping.h"

namespace Bolt
{

	IndexIterator::IndexIterator()
	{
		
	}

	IndexIterator::IndexIterator(const IndexMapping* mapping, int currentIndex)
		: m_Mapping(mapping), m_Ptr(mapping->GetIndexPtr(currentIndex)), m_CurrentIndex(currentIndex)
	{

	}

	const uint* IndexIterator::Get() const
	{ 
		return (uint*)m_Ptr;
	}

	uint* IndexIterator::Get()
	{ 
		return (uint*)m_Ptr; 
	}

	const uint* IndexIterator::operator->() const
	{ 
		return Get(); 
	}

	uint* IndexIterator::operator->()
	{ 
		return Get(); 
	}

	const uint& IndexIterator::operator*() const
	{ 
		return *Get(); 
	}

	uint& IndexIterator::operator*()
	{ 
		return *Get();
	}

	IndexIterator& IndexIterator::operator+=(int amount)
	{
		m_CurrentIndex += amount;
		m_Ptr = m_Mapping->GetIndexPtr(m_CurrentIndex);
		return *this;
	}

	IndexIterator& IndexIterator::operator-=(int amount)
	{
		m_CurrentIndex -= amount;
		m_Ptr = m_Mapping->GetIndexPtr(m_CurrentIndex);
		return *this;
	}

	IndexIterator& IndexIterator::operator++()
	{
		return (*this += 1);
	}

	IndexIterator& IndexIterator::operator--()
	{
		return (*this -= 1);
	}

	IndexIterator& IndexIterator::operator++(int)
	{
		return (*this += 1);
	}

	IndexIterator& IndexIterator::operator--(int)
	{
		return (*this -= 1);
	}

	bool IndexIterator::operator==(const IndexIterator & other) const
	{
		return (m_Ptr == other.m_Ptr && m_Mapping == other.m_Mapping);
	}

	bool IndexIterator::operator!=(const IndexIterator & other) const
	{
		return !(*this == other);
	}

	bool IndexIterator::operator<(const IndexIterator & other) const
	{
		return (m_Ptr < other.m_Ptr);
	}

	bool IndexIterator::operator<=(const IndexIterator & other) const
	{
		return (m_Ptr <= other.m_Ptr);
	}

	bool IndexIterator::operator>(const IndexIterator & other) const
	{
		return (m_Ptr > other.m_Ptr);
	}

	bool IndexIterator::operator>=(const IndexIterator & other) const
	{
		return (m_Ptr >= other.m_Ptr);
	}

}