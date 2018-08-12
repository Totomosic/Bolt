#pragma once
#include "Bolt-Core.h"
#include "../IndexArray.h"

namespace Bolt
{

	class IndexArray;

	template<typename T = uint>
	class BLT_API IndexIterator
	{
	public:
		typedef IndexIterator<T> Iterator;

	public:
		byte* m_Ptr;
		const IndexArray* m_Array;
		int m_CurrentIndex;

	public:
		IndexIterator() = default;
		IndexIterator(byte* ptr, const IndexArray* indexArray, int currentIndex)
			: m_Ptr(ptr), m_Array(indexArray), m_CurrentIndex(currentIndex)
		{
		
		}

		IndexIterator(const Iterator& other) = delete;
		Iterator operator=(const Iterator& other) = delete;

		IndexIterator(Iterator&& other)
			: m_Ptr(other.m_Ptr), m_Array(other.m_Array), m_CurrentIndex(other.m_CurrentIndex)
		{
			other.m_Ptr = nullptr;
		}

		Iterator operator=(Iterator&& other)
		{
			m_Ptr = other.m_Ptr;
			m_Array = other.m_Array;
			m_CurrentIndex = other.m_CurrentIndex;
			other.m_Ptr = nullptr;
			return *this;
		}

		~IndexIterator()
		{
			if (m_Ptr != nullptr)
			{
				m_Array->FreeIterator(*this);
			}
		}

		const T* Get() const { return (T*)m_Ptr; }
		T* Get() { return (T*)m_Ptr; }
		const T* operator->() const { return Get(); }
		T* operator->() { return Get(); }
		const T& operator*() const { return *Get(); }
		T& operator*() { return *Get(); }

		Iterator& operator+=(int amount)
		{
			m_CurrentIndex += amount;
			m_Ptr = m_Array->TestBufferPointer(m_CurrentIndex, m_CurrentIndex - amount, m_Ptr, amount);
			return *this;
		}

		Iterator& operator-=(int amount)
		{
			m_CurrentIndex -= amount;
			m_Ptr = m_Array->TestBufferPointer(m_CurrentIndex, m_CurrentIndex + amount, m_Ptr, -amount);
			return *this;
		}

		Iterator& operator++()
		{
			return (*this += 1);
		}

		Iterator& operator--()
		{
			return (*this -= 1);
		}

		Iterator& operator++(int)
		{
			return (*this += 1);
		}

		Iterator& operator--(int)
		{
			return (*this -= 1);
		}

		bool operator==(const Iterator& other) const
		{
			return (m_Ptr == other.m_Ptr && m_Array == other.m_Array);
		}

		bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

		bool operator<(const Iterator& other) const
		{
			return (m_Ptr < other.m_Ptr);
		}

		bool operator<=(const Iterator& other) const
		{
			return (m_Ptr <= other.m_Ptr);
		}

		bool operator>(const Iterator& other) const
		{
			return (m_Ptr > other.m_Ptr);
		}

		bool operator>=(const Iterator& other) const
		{
			return (m_Ptr >= other.m_Ptr);
		}

		friend class IndexArray;

	};

}