#pragma once
#include "Types.h"

namespace Bolt
{

	class IndexMapping;

	class BLT_API IndexIterator
	{
	public:
		const IndexMapping* m_Mapping;
		void* m_Ptr;
		int m_CurrentIndex;

	public:
		IndexIterator();
		IndexIterator(const IndexMapping* mapping, int currentIndex);

		const uint32_t* Get() const;
		uint32_t* Get();
		const uint32_t* operator->() const;
		uint32_t* operator->();
		const uint32_t& operator*() const;
		uint32_t& operator*();

		IndexIterator& operator+=(int amount);
		IndexIterator& operator-=(int amount);
		IndexIterator& operator++();
		IndexIterator& operator--();
		IndexIterator operator++(int);
		IndexIterator operator--(int);

		friend IndexIterator operator+(const IndexIterator& left, int right);
		friend IndexIterator operator-(const IndexIterator& left, int right);

		bool operator==(const IndexIterator& other) const;
		bool operator!=(const IndexIterator& other) const;
		bool operator<(const IndexIterator& other) const;
		bool operator<=(const IndexIterator& other) const;
		bool operator>(const IndexIterator& other) const;
		bool operator>=(const IndexIterator& other) const;

	};

}