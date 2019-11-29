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

	template<typename T>
	class BLT_API basic_string_view
	{
	public:
		using const_iterator = basic_string_iterator<const T, false>;
		using const_reverse_iterator = basic_string_iterator<const T, true>;

		using size_type = std::size_t;

		static constexpr size_type npos = (size_type)-1;

	private:
		const T* m_Ptr;
		size_type m_Length;

	public:
		constexpr basic_string_view()
			: m_Ptr(nullptr), m_Length(0)
		{
		
		}

		constexpr basic_string_view(const T* str) : basic_string_view(str, CalculateStringLength(str))
		{
			
		}

		constexpr basic_string_view(const T* ptr, size_type length)
			: m_Ptr(ptr), m_Length(length)
		{
		
		}

		constexpr operator std::string_view() const { return std::string_view(m_Ptr, m_Length); }

		constexpr inline size_type size() const { return m_Length; }
		constexpr inline size_type length() const { return m_Length; }
		constexpr inline const T* c_str() const { return m_Ptr; }
		constexpr inline const T* data() const { return m_Ptr; }
		constexpr bool empty() const { return m_Length == 0; }

		constexpr const_iterator begin() const { return const_iterator(GetBufferPtr()); }
		constexpr const_iterator cbegin() const { return const_iterator(GetBufferPtr()); }
		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(GetBufferPtr() + length()); }
		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator(GetBufferPtr() + length()); }
		constexpr const_iterator end() const { return const_iterator(GetBufferPtr() + length()); }
		constexpr const_iterator cend() const { return const_iterator(GetBufferPtr() + length()); }
		constexpr const_reverse_iterator rend() const { return const_reverse_iterator(GetBufferPtr()); }
		constexpr const_reverse_iterator crend() const { return const_reverse_iterator(GetBufferPtr()); }

		constexpr inline const T& front() const { return (*this)[0]; }
		constexpr inline T& front() { return (*this)[0]; }
		constexpr inline const T& back() const { return (*this)[length() - 1]; }
		constexpr inline T& back() { return (*this)[length() - 1]; }

		constexpr const T& operator[](size_type index) const { return GetBufferPtr()[index]; }
		constexpr T& operator[](size_type index) { return GetBufferPtr()[index]; }
		constexpr const T& at(size_type index) const { return GetBufferPtr()[index]; }
		constexpr T& at(size_type index) { return GetBufferPtr()[index]; }

		//constexpr size_type copy(T* s, size_type n, size_type pos = 0) const;
		constexpr basic_string_view<T> substr(size_type pos = 0, size_type n = npos) const;
		constexpr int compare(basic_string_view<T> s) const noexcept;
		constexpr int compare(size_type pos1, size_type n1, basic_string_view<T> s) const;
		constexpr int compare(size_type pos1, size_type n1, basic_string_view<T> s, size_type pos2, size_type n2) const;
		constexpr int compare(const T* s) const;
		constexpr int compare(size_type pos1, size_type n1, const T* s) const;
		constexpr int compare(size_type pos1, size_type n1, const T* s, size_type n2) const;

		constexpr bool starts_with(basic_string_view<T> x) const noexcept
		{
			if constexpr (x.length() > length())
				return false;
			for (constexpr int i = 0; i < x.length(); i++)
			{
				if constexpr ((*this)[i] != x[i])
					return false;
			}
			return true;
		}
		constexpr bool starts_with(T x) const noexcept
		{
			if constexpr (empty())
				return false;
			return front() == x;
		}
		constexpr bool starts_with(const T* x) const { return starts_with(basic_string_view<T>(x)); }

		constexpr bool ends_with(basic_string_view<T> x) const noexcept
		{
			if constexpr (x.length() > length())
				return false;
			for (constexpr int i = 0; i < x.length(); i++)
			{
				if constexpr ((*this)[length() - x.length() + i] != x[i])
					return false;
			}
			return true;
		}
		constexpr bool ends_with(T x) const noexcept
		{
			return (empty()) ? false : back() == x;
		}
		constexpr bool ends_with(const T* x) const { return ends_with(basic_string_view<T>(x)); }

		constexpr size_type find(basic_string_view<T> s, size_type pos = 0) const noexcept
		{
			if constexpr (s.length() + pos > length())
				return npos;
			for (constexpr i = pos; i < length(); i++)
			{
				constexpr bool found = true;
				for (constexpr j = 0; j < s.length(); j++)
				{

				}
				
			}
		}

		constexpr size_type find(T c, size_type pos = 0) const noexcept;
		constexpr size_type find(const T* s, size_type pos, size_type n) const;
		constexpr size_type find(const T* s, size_type pos = 0) const;

		constexpr size_type rfind(basic_string_view<T> s, size_type pos = npos) const noexcept;
		constexpr size_type rfind(T c, size_type pos = npos) const noexcept;
		constexpr size_type rfind(const T* s, size_type pos, size_type n) const;
		constexpr size_type rfind(const T* s, size_type pos = npos) const;

		constexpr size_type find_first_of(basic_string_view<T> s, size_type pos = 0) const noexcept;
		constexpr size_type find_first_of(T c, size_type pos = 0) const noexcept;
		constexpr size_type find_first_of(const T* s, size_type pos, size_type n) const;
		constexpr size_type find_first_of(const T* s, size_type pos = 0) const;

		constexpr size_type find_last_of(basic_string_view<T> s, size_type pos = npos) const noexcept;
		constexpr size_type find_last_of(T c, size_type pos = npos) const noexcept;
		constexpr size_type find_last_of(const T* s, size_type pos, size_type n) const;
		constexpr size_type find_last_of(const T* s, size_type pos = npos) const;

		constexpr size_type find_first_not_of(basic_string_view<T> s, size_type pos = 0) const noexcept;
		constexpr size_type find_first_not_of(T c, size_type pos = 0) const noexcept;
		constexpr size_type find_first_not_of(const T* s, size_type pos, size_type n) const;
		constexpr size_type find_first_not_of(const T* s, size_type pos = 0) const;

		constexpr size_type find_last_not_of(basic_string_view<T> s, size_type pos = npos) const noexcept;
		constexpr size_type find_last_not_of(T c, size_type pos = npos) const noexcept;
		constexpr size_type find_last_not_of(const T* s, size_type pos, size_type n) const;
		constexpr size_type find_last_not_of(const T* s, size_type pos = npos) const;

	private:
		constexpr inline const T* GetBufferPtr() const { return m_Ptr; }
		constexpr size_type CalculateStringLength(const T* str) const
		{
			return *str ? 1 + CalculateStringLength(str + 1) : 0;
		}

	};

	template<typename T>
	class BLT_API basic_string
	{
	public:
		using iterator					= basic_string_iterator<T, false>;
		using const_iterator			= basic_string_iterator<const T, false>;
		using reverse_iterator			= basic_string_iterator<T, true>;
		using const_reverse_iterator	= basic_string_iterator<const T, true>;

		using size_type = std::size_t;

		static constexpr size_type npos = (size_type)-1;

	private:
		T* m_Buffer;
		size_type m_Capacity;
		size_type m_Length;

	public:
		basic_string()
			: m_Buffer(nullptr), m_Capacity(0), m_Length(0)
		{
		
		}

		basic_string(const T* buffer, size_type n)
			: m_Buffer(nullptr), m_Capacity(n), m_Length(n)
		{
			ReallocateBuffer(m_Capacity + 1);
			memcpy(GetBufferPtr(), buffer, n * sizeof(T));
			NullTerminate();
		}

		basic_string(const T* string) : basic_string(string, GetStringLength(string))
		{
		
		}

		basic_string(const basic_string<T>& other)
			: m_Buffer(nullptr), m_Capacity(other.length()), m_Length(other.length())
		{
			ReallocateBuffer(m_Capacity + 1);
			memcpy(GetBufferPtr(), other.data(), m_Length * sizeof(T));
			NullTerminate();
		}

		basic_string<T>& operator=(const basic_string<T>& other)
		{
			if (m_Buffer != nullptr)
			{
				delete[] m_Buffer;
			}
			m_Buffer = nullptr;
			m_Capacity = other.length();
			m_Length = other.length();
			ReallocateBuffer(m_Capacity + 1);
			memcpy(GetBufferPtr(), other.data(), m_Length * sizeof(T));
			NullTerminate();
			return *this;
		}

		basic_string(basic_string<T>&& other)
			: m_Buffer(other.m_Buffer), m_Capacity(other.m_Capacity), m_Length(other.m_Length)
		{
			other.m_Buffer = nullptr;
		}

		basic_string<T>& operator=(basic_string<T>&& other)
		{
			T* buffer = m_Buffer;
			m_Buffer = other.m_Buffer;
			m_Length = other.m_Length;
			m_Capacity = other.m_Capacity;
			other.m_Buffer = buffer;
			return *this;
		}

		~basic_string()
		{
			if (m_Buffer != nullptr)
			{
				delete[] m_Buffer;
			}
		}

		inline size_type size() const { return m_Length; }
		inline size_type length() const { return size(); }
		inline size_type capacity() const { return m_Capacity; }

		inline const T* c_str() const { return GetBufferPtr(); }
		inline const T* data() const { return GetBufferPtr(); }
		inline std::string cpp_str() const { return std::string(GetBufferPtr(), length()); }
		bool empty() const { return length() == 0; }

		iterator begin() const { return iterator(GetBufferPtr()); }
		const_iterator cbegin() const { return const_iterator(GetBufferPtr()); }
		reverse_iterator rbegin() const { return reverse_iterator(GetBufferPtr() + length()); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(GetBufferPtr() + length()); }
		iterator end() const { return iterator(GetBufferPtr() + length()); }
		const_iterator cend() const { return const_iterator(GetBufferPtr() + length()); }
		reverse_iterator rend() const { return reverse_iterator(GetBufferPtr()); }
		const_reverse_iterator crend() const { return const_reverse_iterator(GetBufferPtr()); }

		inline const T& front() const { return (*this)[0]; }
		inline T& front() { return (*this)[0]; }
		inline const T& back() const { return (*this)[length() - 1]; }
		inline T& back() { return (*this)[length() - 1]; }

		const T& operator[](size_type index) const { return GetBufferPtr()[index]; }
		T& operator[](size_type index) { return GetBufferPtr()[index]; }
		const T& at(size_type index) const { return GetBufferPtr()[index]; }
		T& at(size_type index) { return GetBufferPtr()[index]; }

		basic_string<T>& erase(size_type index, size_type count = npos)
		{
			if (count > length() - index)
			{
				count = length() - index;
			}
			ShuffleForward(index + count, count);
			m_Length -= count;
			NullTerminate();
			return *this;
		}

		iterator erase(const_iterator position)
		{
			return erase(position, position + 1);
		}

		iterator erase(const_iterator begin, const_iterator end)
		{
			const_iterator b = cbegin();
			size_type start = (size_type)(begin.get() - b.get());
			erase(start, (size_type)(end.get() - begin.get()));
			return this->begin() + start;
		}

	private:
		inline T* GetBufferPtr() const { return m_Buffer; }

		void ReallocateBuffer(size_type capacity)
		{
			if (capacity <= m_Capacity)
			{
				return;
			}
			T* buffer = new T[capacity];
			if (m_Buffer != nullptr && buffer != nullptr)
			{
				memcpy(buffer, GetBufferPtr(), m_Length);
				delete[] m_Buffer;
			}
			m_Buffer = buffer;
			m_Capacity = capacity;
		}

		void NullTerminate()
		{
			ReallocateBuffer(m_Length + 1);
			GetBufferPtr()[m_Length] = (T)0;
		}

		size_type GetStringLength(const T* string)
		{
			size_type length = 0;
			while (string[length++] != (T)0) {}
			return length - 1;
		}

		void ShuffleForward(size_type index, size_type length)
		{
			if (length == 0)
			{
				return;
			}
			std::memmove(GetBufferPtr() + index - length, GetBufferPtr() + index, (size() - index) * sizeof(T));
		}

		void ShuffleBackward(size_type index, size_type length)
		{
			if (length == 0)
			{
				return;
			}
			ReallocateBuffer(size() + length + 1);
			std::memmove(GetBufferPtr() + index + length, GetBufferPtr() + index, (size() - index) * sizeof(T));
		}

	};

	//using string = basic_string<char>;

}