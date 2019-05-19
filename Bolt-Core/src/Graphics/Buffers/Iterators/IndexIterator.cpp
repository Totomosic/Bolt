#include "bltpch.h"
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

	const uint32_t* IndexIterator::Get() const
	{ 
		return (uint32_t*)m_Ptr;
	}

	uint32_t* IndexIterator::Get()
	{ 
		return (uint32_t*)m_Ptr; 
	}

	const uint32_t* IndexIterator::operator->() const
	{ 
		return Get(); 
	}

	uint32_t* IndexIterator::operator->()
	{ 
		return Get(); 
	}

	const uint32_t& IndexIterator::operator*() const
	{ 
		return *Get(); 
	}

	uint32_t& IndexIterator::operator*()
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

	IndexIterator IndexIterator::operator++(int)
	{
		(*this) += 1;
		return (*this - 1);
	}

	IndexIterator IndexIterator::operator--(int)
	{
		(*this) -= 1;
		return (*this + 1);
	}

	IndexIterator& IndexIterator::operator++()
	{
		return (*this += 1);
	}

	IndexIterator& IndexIterator::operator--()
	{
		return (*this -= 1);
	}

	IndexIterator operator+(const IndexIterator& left, int right)
	{
		return IndexIterator(left.m_Mapping, left.m_CurrentIndex + right);
	}

	IndexIterator operator-(const IndexIterator& left, int right)
	{
		return (left + (-right));
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