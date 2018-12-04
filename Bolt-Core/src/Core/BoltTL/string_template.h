#pragma once
#include "Iterator.h"
#include <string>
#include "BoltDefines.h"

namespace blt
{

	template<typename From, typename To>
	struct BLT_API char_conversion
	{
	public:
		To* operator()(const From* str, size_t chrCount)
		{
			BLT_ASSERT(false, "unsupported character conversion");
			return nullptr;
		}
	};

	template<>
	struct BLT_API char_conversion<char, wchar_t>
	{
	public:
		wchar_t* operator()(const char* str, size_t chrCount)
		{
			wchar_t* buffer = (wchar_t*)std::realloc(nullptr, chrCount * sizeof(wchar_t));
			mbstowcs(buffer, str, chrCount);
			return buffer;
		}
	};

	template<>
	struct BLT_API char_conversion<wchar_t, char>
	{
	public:
		char* operator()(const wchar_t* str, size_t chrCount)
		{
			char* buffer = (char*)std::realloc(nullptr, chrCount * sizeof(char));
			wcstombs(buffer, str, chrCount);
			return buffer;
		}
	};

	template<>
	struct BLT_API char_conversion<char, char>
	{
	public:
		char* operator()(const char* str, size_t chrCount)
		{
			char* buffer = (char*)std::realloc(nullptr, chrCount * sizeof(char));
			memcpy(buffer, str, chrCount * sizeof(char));
			return buffer;
		}
	};

	template<>
	struct BLT_API char_conversion<wchar_t, wchar_t>
	{
	public:
		wchar_t* operator()(const wchar_t* str, size_t chrCount)
		{
			wchar_t* buffer = (wchar_t*)std::realloc(nullptr, chrCount * sizeof(wchar_t));
			memcpy(buffer, str, chrCount * sizeof(wchar_t));
			return buffer;
		}
	};



	template<typename _Ty>
	class BLT_API string_template
	{
	public:
		using value_t = _Ty;
		using const_value_t = const value_t;
		using pointer_t = value_t * ;
		using const_pointer_t = const pointer_t;
		using ref_t = value_t & ;
		using const_ref_t = const ref_t;

		using str_iterator = blt::iterator<value_t>;
		using const_str_iterator = blt::iterator<const_value_t>;

		using this_t = string_template<value_t>;

	public:
		static constexpr size_t type_size = sizeof(value_t);
		static constexpr size_t npos = (size_t)-1;

	private:
		pointer_t m_Buffer;
		size_t m_BufferCapacityCount;
		size_t m_CharCount;

	public:
		string_template() : string_template(1)
		{

		}

		string_template(size_t charCapacity)
			: m_Buffer(nullptr), m_BufferCapacityCount(0), m_CharCount(0)
		{
			realloc_buffer(charCapacity);
			null_terminate();
		}

		string_template(const_ref_t chr) : string_template((size_t)2)
		{
			push_back(chr);
		}

		string_template(const_pointer_t str) : string_template(calc_string_length(str) + 1)
		{
			append(str);
		}

		string_template(const char* str) : string_template()
		{
			size_t length = strlen(str);
			append(char_conversion<char, value_t>()(str, length + 1), length);
		}

		string_template(const std::string& str) : string_template(str.c_str())
		{
			
		}

		string_template(const_pointer_t str, size_t count) : string_template(count)
		{
			append(str, count);
		}

		template<typename OtherChr>
		string_template(const string_template<OtherChr>& other)
			: m_Buffer(nullptr), m_BufferCapacityCount(0), m_CharCount(0)
		{
			pointer_t buffer = char_conversion<OtherChr, value_t>()(other.data(), other.size() + 1);
			append(buffer, other.size());
			std::free(buffer);
		}

		template<typename OtherChr>
		this_t& operator=(const string_template<OtherChr>& other)
		{
			m_Buffer = nullptr;
			m_BufferCapacityCount = 0;
			m_CharCount = 0;
			pointer_t buffer = char_conversion<OtherChr, value_t>()(other.data(), other.size() + 1);
			append(buffer, other.size());
			std::free(buffer);
			return *this;
		}

		template<typename OtherChr>
		string_template(string_template<OtherChr>&& other)
			: m_Buffer(char_conversion<OtherChr, value_t>()(other.m_Buffer, other.capacity())), m_BufferCapacityCount(other.m_BufferCapacityCount), m_CharCount(other.m_CharCount)
		{
			other.m_Buffer = nullptr;
		}

		template<typename OtherChr>
		this_t& operator=(string_template<OtherChr>&& other)
		{
			pointer_t buff = m_Buffer;
			m_Buffer = char_conversion<OtherChr, value_t>()(other.m_Buffer, other.capacity());
			std::free(other.m_Buffer);
			m_BufferCapacityCount = other.m_BufferCapacityCount;
			m_CharCount = other.m_CharCount;
			other.m_Buffer = char_conversion<value_t, OtherChr>()(buff, capacity());
			return *this;
		}

		~string_template()
		{
			if (m_Buffer != nullptr)
			{
				std::free((void*)m_Buffer);
			}
		}

		template<typename OtherChr>
		operator string_template<OtherChr>()
		{
			return string_template<OtherChr>(char_conversion<value_t, OtherChr>(data(), size() + 1), size());
		}

		// Return number of characters in string
		inline size_t size() const { return m_CharCount; }
		// Return number of characters in string
		inline size_t length() const { return size(); }
		// Return maximum number of characters that can be stored in buffer
		inline size_t capacity() const { return m_BufferCapacityCount; }
		// Return number of bytes used in string buffer
		inline size_t buffer_size() const { return m_CharCount * type_size; }
		// Return number of bytes used in string buffer
		inline size_t buffer_length() const { return buffer_size(); }
		// Return maximum number of bytes that can be stored in buffer
		inline size_t buffer_capacity() const { return capacity() * type_size; }
		// Return c style representation of string
		inline const_pointer_t c_str() const { return buffer_ptr(); }
		// Return pointer to beginning of string buffer
		inline const_pointer_t data() const { return buffer_ptr(); }
		// Return whether the string is empty (size() == 0)
		inline bool empty() const { return size() == 0; }

		// Returns iterator to start of string
		inline const_str_iterator begin() const { return const_str_iterator(buffer_ptr()); }
		// Returns iterator to start of string
		inline str_iterator begin() { return str_iterator(buffer_ptr()); }
		// Returns iterator to character after the last character of the string (likely null terminator)
		inline const_str_iterator end() const { return const_str_iterator(buffer_ptr() + length()); }
		// Returns iterator to character after the last character of the string (likely null terminator)
		inline str_iterator end() { return str_iterator(buffer_ptr() + length()); }

		// Returns reference to first character of string
		inline const_ref_t front() const { return *(begin()); }
		// Returns reference to first character of string
		inline ref_t front() { return *(begin()); }
		// Returns reference to last character of string
		inline const_ref_t back() const { return *(end() - 1); }
		// Returns reference to last character of string
		inline ref_t back() { return *(end() - 1); }

		// Returns reference to character at given index
		inline const_ref_t at(size_t index) const { return *(begin() + index); }
		// Returns reference to character at given index
		inline ref_t at(size_t index) { return *(begin() + index); }
		// Returns reference to character at given index
		inline const_ref_t operator[](size_t index) const { return at(index); }
		// Returns reference to character at given index
		inline ref_t operator[](size_t index) { return at(index); }

		// Adds a new character to the end of the string
		void push_back(const_ref_t chr)
		{
			test_realloc_buffer(size() + 1);
			buffer_ptr()[size()] = chr;
			m_CharCount++;
			null_terminate();
		}

		// Removes the last character of the string
		void pop_back()
		{
			BLT_ASSERT(size() > 0, "string is empty");
			m_CharCount--;
			null_terminate();
		}

		// Removes the first character of the string
		void pop_front()
		{
			BLT_ASSERT(size() > 0, "string is empty");
			move_forward(1, 1);
		}

		// Appends given character to end of string
		this_t& append(const_ref_t chr)
		{
			push_back(chr);
			return *this;
		}

		// Appends given null-terminated string to end of string
		this_t& append(const_pointer_t str)
		{
			size_t length = calc_string_length(str);
			return append(str, length);
		}

		// Appends given string to end of string
		this_t& append(const this_t& str)
		{
			return append(str.data(), str.size());
		}

		// Appends characters from buffer to end of string
		this_t& append(const_pointer_t buffer, size_t chrCount)
		{
			test_realloc_buffer(size() + chrCount);
			memcpy(buffer_ptr() + size(), buffer, chrCount * type_size);
			m_CharCount += chrCount;
			null_terminate();
			return *this;
		}
		
		// Inserts character at given index in string
		this_t& insert(size_t index, const_ref_t chr)
		{
			BLT_ASSERT(index < size() + 1, "cannot insert past end of string");
			test_realloc_buffer(size() + 1);
			move_backward(index, 1);
			buffer_ptr()[index] = chr;
			return *this;
		}

		// Inserts given null-terminated string at given index in string
		this_t& insert(size_t index, const_pointer_t str)
		{
			size_t length = calc_string_length(str);
			return insert(index, str, length);
		}

		// Inserts given string at given index in string
		this_t& insert(size_t index, const this_t& str)
		{
			return insert(index, str.data(), str.size());
		}

		// Inserts characters from buffer at given index in string
		this_t& insert(size_t index, const_pointer_t buffer, size_t chrCount)
		{
			BLT_ASSERT(index < size() + 1, "cannot insert past end of string");
			test_realloc_buffer(size() + chrCount);
			move_backward(index, chrCount);
			memcpy(buffer_ptr() + index, buffer, (chrCount)* type_size);
			return *this;
		}

		// Returns new string with right string appended to end of left string
		friend this_t operator+(const this_t& left, const this_t& right)
		{
			this_t str = left;
			str.append(right);
			return str;
		}

		// Returns new string with right string appended to end of left string
		friend this_t operator+(const this_t& left, const_pointer_t right)
		{
			this_t str = left;
			str.append(right);
			return str;
		}

		// Returns new string with right character appended to end of left string
		friend this_t operator+(const this_t& left, const_ref_t chr)
		{
			this_t str = left;
			str.append(chr);
			return str;
		}

		// Returns new string with right string appended to end of left string
		friend this_t operator+(const_pointer_t left, const this_t& right)
		{
			this_t str = this_t(left);
			str.append(right);
			return str;
		}

		// Returns new string with right string appended to left character
		friend this_t operator+(const_ref_t left, const this_t& right)
		{
			this_t str = this_t(left);
			str.append(right);
			return str;
		}

		// Appends given string to end of this string
		this_t& operator+=(const this_t& right)
		{
			return append(right);
		}

		// Appends given null-terminated string to end of this string
		this_t& operator+=(const_pointer_t right)
		{
			return append(right);
		}

		// Appends given character to end of this string
		this_t& operator+=(const_ref_t right)
		{
			return append(right);
		}

		// Compares the contents of two strings
		friend bool operator==(const this_t& left, const this_t& right)
		{
			if (left.size() != right.size())
			{
				return false;
			}
			for (size_t i = 0; i < left.size(); i++)
			{
				if (left.at(i) != right.at(i))
				{
					return false;
				}
			}
			return true;
		}

		// Compares the contents of two strings
		friend bool operator==(const this_t& left, const_pointer_t right)
		{
			return (left == this_t(right));
		}

		// Compares string to character
		friend bool operator==(const this_t& left, const_ref_t right)
		{
			if (left.size() != 1)
			{
				return false;
			}
			return left.at(0) == right;
		}

		// Compares the contents of two strings
		friend bool operator==(const_pointer_t left, const this_t& right)
		{
			return right == left;
		}

		// Compares string to character
		friend bool operator==(const_ref_t left, const this_t& right)
		{
			return right == left;
		}

		// Compares the contents of two strings
		friend bool operator!=(const this_t& left, const this_t& right)
		{
			return !(left == right);
		}

		// Compares the contents of two strings
		friend bool operator!=(const this_t& left, const_pointer_t right)
		{
			return !(left == right);
		}

		// Compares string to character
		friend bool operator!=(const this_t& left, const_ref_t right)
		{
			return !(left == right);
		}

		// Compares the contents of two strings
		friend bool operator!=(const_pointer_t left, const this_t& right)
		{
			return !(left == right);
		}

		// Compares string to character
		friend bool operator!=(const_ref_t left, const this_t& right)
		{
			return !(left == right);
		}

		// Erase characters from string, if count == npos, erases to end of string
		this_t& erase(size_t index, size_t count)
		{
			if (count == this_t::npos)
			{
				count = size() - index;
			}
			BLT_ASSERT(index + count < size(), "cannot erase that many characters");
			move_forward(index, count);
			return *this;
		}

		// Erase character at iterator
		this_t& erase(str_iterator chr)
		{
			size_t index = index_of_iterator(chr);
			return erase(index, 1);
		}

		// Erase character range [beginChr, lastChr] (do not use string::end())
		this_t& erase(str_iterator beginChr, str_iterator lastChr)
		{
			size_t beginIndex = index_of_iterator(beginChr);
			size_t lastIndex = index_of_iterator(lastChr);
			BLT_ASSERT(lastIndex > beginIndex, "begin iterator was after last iterator");
			return erase(beginIndex, lastIndex - beginIndex + 1);
		}

		// Returns index of first occurrence of given string after start
		size_t find(const this_t& str, size_t start = 0)
		{
			return find(str.data(), str.size(), start);
		}

		// Returns index of first occurrence of given string after start
		size_t find(const_pointer_t str, size_t start = 0)
		{
			return find(str, calc_string_length(str), start);
		}

		// Returns index of first occurrence of characters from buffer after start
		size_t find(const_pointer_t str, size_t chrCount, size_t start = 0)
		{
			BLT_ASSERT(chrCount < size(), "string is shorter than finding string");
			BLT_ASSERT(start < size() - chrCount, "cannot start past end of string");
			for (size_t i = 0; i < size() - chrCount; i++)
			{
				bool found = true;
				for (size_t j = 0; j < chrCount; j++)
				{
					if (at(i + j) != str[j])
					{
						found = false;
						break;
					}
				}
				if (found)
				{
					return i;
				}
			}
			return npos;
		}

		// Puts string into stream
		friend std::ostream& operator<<(std::ostream& stream, const this_t& str)
		{
			stream << str.c_str();
			return stream;
		}

		// Puts string into stream
		friend std::wostream& operator<<(std::wostream& stream, const this_t& str)
		{
			stream << str.c_str();
			return stream;
		}

		template<typename> friend class string_template;

	private:
		// Returns pointer to start of string buffer
		inline pointer_t buffer_ptr() const { return m_Buffer; }
		// Calculates the character count of a given string
		inline size_t calc_string_length(const_pointer_t str) const
		{
			size_t length = 0;
			while (str[length] != (value_t)'\0')
			{
				length++;
			}
			return length;
		}

		// Reallocates the string buffer is the given number of characters will not fit (characterCount should not include the null terminator)
		bool test_realloc_buffer(size_t characterCount)
		{
			if ((characterCount + 1) * type_size > buffer_capacity())
			{
				realloc_buffer((characterCount + 1) * type_size);
				return true;
			}
			return false;
		}

		// Reallocates the string buffer to a given capacity (char count)
		void realloc_buffer(size_t newCapacity)
		{
			m_Buffer = (pointer_t)std::realloc(m_Buffer, newCapacity * type_size);
			m_BufferCapacityCount = newCapacity;
		}

		// Append the null terminator character at the end of the string
		void null_terminate()
		{
			buffer_ptr()[size()] = (value_t)'\0';
		}

		// Moves all characters after index forward by count positions (and null terminates)
		void move_forward(size_t index, size_t count)
		{
			BLT_ASSERT(index - count > 0, "cannot move forward past the front of buffer");
			memmove(buffer_ptr() + (index - count), buffer_ptr() + index, (size() - index) * type_size);
			m_CharCount -= count;
			null_terminate();
		}

		// Moves all characters after index backward by count positions (and null terminates)
		void move_backward(size_t index, size_t count)
		{
			test_realloc_buffer(size() + count);
			memmove(buffer_ptr() + index + count, buffer_ptr() + index, (size() - index) * type_size);
			m_CharCount += count;
			null_terminate();
		}

		// Returns index that iterator corresponds to
		size_t index_of_iterator(const_str_iterator iterator) const
		{
			size_t iptr = (size_t)iterator.get();
			size_t bptr = (size_t)begin().get();
			BLT_ASSERT(iptr > bptr, "iterator is before beginning of string");
			return (iptr - bptr) / type_size;
		}

		// Returns index that iterator corresponds to
		size_t index_of_iterator(str_iterator iterator)
		{
			size_t iptr = (size_t)iterator.get();
			size_t bptr = (size_t)begin().get();
			BLT_ASSERT(iptr > bptr, "iterator is before beginning of string");
			return (iptr - bptr) / type_size;
		}

	};

	using wstring = string_template<wchar_t>;

}