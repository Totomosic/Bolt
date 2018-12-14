#pragma once
#include <iterator>
#include "Logging.h"

namespace blt
{

	template<typename _Val, typename _Ptr = _Val*, typename _Ref = _Val&>
	class iterator
	{
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef _Val value_type;
		typedef _Ptr pointer;
		typedef _Ref reference;
		typedef const _Ref const_reference;
		typedef std::ptrdiff_t difference_type;

		typedef iterator<_Val, _Ptr, _Ref> this_iterator;

	private:
		pointer m_Ptr;

	public:
		iterator(pointer ptr)
			: m_Ptr(ptr)
		{
		
		}

		iterator(const this_iterator& other) = default;
		iterator(this_iterator&& other) = default;
		this_iterator& operator=(const this_iterator& other) = default;
		this_iterator& operator=(this_iterator&& other) = default;

		inline const_reference operator*() const { return *m_Ptr; }
		inline reference operator*() { return *m_Ptr; }

		inline pointer get() const { return m_Ptr; }
		inline const pointer operator->() const { return m_Ptr; }
		inline pointer operator->() { return m_Ptr; }

		friend this_iterator operator+(const this_iterator& iterator, difference_type value)
		{
			return this_iterator(iterator.m_Ptr + value);
		}

		friend this_iterator operator+(difference_type value, const this_iterator& iterator)
		{
			return iterator + value;
		}

		friend this_iterator operator-(const this_iterator& iterator, difference_type value)
		{
			return this_iterator(iterator.m_Ptr - value);
		}

		this_iterator& operator+=(difference_type value)
		{
			m_Ptr += value;
			return *this;
		}

		this_iterator& operator-=(difference_type value)
		{
			m_Ptr -= value;
			return *this;
		}

		this_iterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		this_iterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		bool operator==(const this_iterator& iterator) const
		{
			return m_Ptr == iterator.m_Ptr;
		}

		bool operator==(difference_type index) const
		{
			return m_Ptr == (pointer)index;
		}

		bool operator!=(const this_iterator& iterator) const
		{
			return !(*this == iterator);
		}

		bool operator!=(difference_type index) const
		{
			return !(*this == index);
		}

		bool operator<(const this_iterator& iterator) const
		{
			return m_Ptr < iterator.m_Ptr;
		}

		bool operator<=(const this_iterator& iterator) const
		{
			return m_Ptr <= iterator.m_Ptr;
		}

		bool operator>(const this_iterator& iterator) const
		{
			return m_Ptr > iterator.m_Ptr;
		}

		bool operator>=(const this_iterator& iterator) const
		{
			return m_Ptr >= iterator.m_Ptr;
		}

	};

}