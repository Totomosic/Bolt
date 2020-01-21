#pragma once
#include <string>
#include <string_view>
#include <iterator>
#include <stdint.h>
#include "BoltDefines.h"

namespace blt
{

	template<typename T, bool Reversed = false>
	class BLT_API basic_string_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::size_t;

	private:
		T* m_Ptr;

	public:
		basic_string_iterator(T* ptr)
			: m_Ptr(ptr)
		{
		}

		inline T& operator*() const { return *(m_Ptr + GetOffset()); }
		inline T* operator->() const { return m_Ptr + GetOffset(); }
		inline T* get() const { return m_Ptr + GetOffset(); }

		friend basic_string_iterator<T, Reversed> operator+(const basic_string_iterator<T, Reversed>& it, difference_type other)
		{
			return basic_string_iterator<T, Reversed>(IncrementPtr(it.get(), other));
		}

		friend basic_string_iterator<T, Reversed> operator+(difference_type other, const basic_string_iterator<T, Reversed>& it)
		{
			return it + other;
		}

		friend basic_string_iterator<T, Reversed> operator-(const basic_string_iterator<T, Reversed>& it, difference_type other)
		{
			return basic_string_iterator<T, Reversed>(DecrementPtr(it.get(), other));
		}

		basic_string_iterator<T, Reversed>& operator+=(difference_type amount)
		{
			m_Ptr = IncrementPtr(m_Ptr, amount);
			return *this;
		}

		basic_string_iterator<T, Reversed>& operator-=(difference_type amount)
		{
			m_Ptr = DecrementPtr(m_Ptr, amount);
			return *this;
		}

		basic_string_iterator<T, Reversed>& operator++()
		{
			return (*this) += 1;
		}

		basic_string_iterator<T, Reversed>& operator--()
		{
			return (*this) -= 1;
		}

		basic_string_iterator<T, Reversed> operator++(int)
		{
			T* ptr = m_Ptr;
			m_Ptr = IncrementPtr(m_Ptr, 1);
			return basic_string_iterator<T, Reversed>(ptr);
		}

		basic_string_iterator<T, Reversed> operator--(int)
		{
			T* ptr = m_Ptr;
			m_Ptr = DecrementPtr(m_Ptr, 1);
			return basic_string_iterator<T, Reversed>(ptr);
		}

		template<bool R>
		friend bool operator==(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, R>& other)
		{
			return it.get() == other.get();
		}

		template<bool R>
		friend bool operator!=(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, R>& other)
		{
			return it.get() != other.get();
		}

		friend bool operator<=(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, Reversed>& other)
		{
			if (!Reversed)
			{
				return it.get() <= other.get();
			}
			return it.get() >= other.get();
		}

		friend bool operator<(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, Reversed>& other)
		{
			if (!Reversed)
			{
				return it.get() < other.get();
			}
			return it.get() > other.get();
		}

		friend bool operator>=(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, Reversed>& other)
		{
			if (!Reversed)
			{
				return it.get() >= other.get();
			}
			return it.get() <= other.get();
		}

		friend bool operator>(const basic_string_iterator<T, Reversed>& it, const basic_string_iterator<T, Reversed>& other)
		{
			if (!Reversed)
			{
				return it.get() > other.get();
			}
			return it.get() < other.get();
		}

	private:
		static T* IncrementPtr(T* ptr, difference_type amount)
		{
			if (!Reversed)
			{
				return ptr + amount;
			}
			return ptr - amount;
		}

		static T* DecrementPtr(T* ptr, difference_type amount)
		{
			if (!Reversed)
			{
				return ptr - amount;
			}
			return ptr + amount;
		}

		static constexpr int GetOffset()
		{
			return (Reversed) ? -1 : 0;
		}

	};

}