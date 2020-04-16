#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename T>
	class BLT_API IndexIterator
	{
	public:
		T* m_Buffer;

	public:
		IndexIterator() : IndexIterator(nullptr) {}
		IndexIterator(T* buffer)
			: m_Buffer(buffer)
		{
		}

		inline const T* Get() const { return m_Buffer; }
		inline T* Get() { return m_Buffer; }
		inline const T* operator->() const { return m_Buffer; }
		inline T* operator->() { return m_Buffer; }
		inline const T& operator*() const { return *m_Buffer; }
		inline T& operator*() { return *m_Buffer; }

		IndexIterator<T>& operator+=(int amount)
		{
			m_Buffer += amount;
			return *this;
		}

		IndexIterator<T>& operator-=(int amount)
		{
			m_Buffer -= amount;
			return *this;
		}

		IndexIterator<T>& operator++()
		{
			return ((*this) += 1);
		}

		IndexIterator<T>& operator--()
		{
			return ((*this) -= 1);
		}

		IndexIterator<T> operator++(int)
		{
			(*this) += 1;
			return (*this) - 1;
		}

		IndexIterator<T> operator--(int)
		{
			(*this) -= 1;
			return (*this) + 1;
		}

		friend IndexIterator<T> operator+(const IndexIterator<T>& left, int right)
		{
			return IndexIterator<T>(left.m_Buffer + right);
		}

		friend IndexIterator<T> operator-(const IndexIterator<T>& left, int right)
		{
			return IndexIterator<T>(left.m_Buffer - right);
		}

		bool operator==(const IndexIterator<T>& other) const
		{
			return m_Buffer == other.m_Buffer;
		}

		bool operator!=(const IndexIterator<T>& other) const
		{
			return !((*this) == other);
		}

		bool operator<(const IndexIterator<T>& other) const
		{
			return m_Buffer < other.m_Buffer;
		}

		bool operator<=(const IndexIterator<T>& other) const
		{
			return m_Buffer <= other.m_Buffer;
		}

		bool operator>(const IndexIterator<T>& other) const
		{
			return m_Buffer > other.m_Buffer;
		}

		bool operator>=(const IndexIterator<T>& other) const
		{
			return m_Buffer >= other.m_Buffer;
		}

	};

}