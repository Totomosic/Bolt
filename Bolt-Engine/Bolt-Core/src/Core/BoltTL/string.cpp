#include "bltpch.h"

#include "string.h"

#if BLT_USE_STRING

#define BLT_MAX(a, b) (((b) > (a)) ? (b) : (a))

namespace blt
{

	string::string() : string((uint32_t)1)
	{

	}

	string::string(uint32_t capacity)
		: m_Buffer(nullptr), m_Size(0), m_Capacity(capacity)
	{
		realloc_buffer(capacity);
	}

	string::string(char c) : string((uint32_t)2)
	{
		append(c);
	}

	string::string(const char* str) : string()
	{
		append(str);
	}

	string::string(const char* buffer, unsigned int size) : string(size + (uint32_t)1)
	{
		memcpy(buffer_ptr(), buffer, size);
		m_Size += size;
		null_terminate();
	}

	string::string(const std::string& str) : string(str.c_str())
	{

	}

	string::string(std::initializer_list<char> il) : string((uint32_t)il.size() + (uint32_t)1)
	{
		const char* ptr = il.begin();
		uint32_t i = 0;
		while (ptr < il.end())
		{
			buffer_ptr()[i] = *ptr;
			i++;
			ptr++;
		}
		m_Size = i;
		null_terminate();
	}

	string::string(const string& other) : string(other.size() + 1)
	{
		append(other.c_str(), other.size());
	}

	string::string(string&& other) noexcept
	{
		m_Buffer = other.m_Buffer;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		other.m_Buffer = nullptr;
	}

	string::~string()
	{
		std::free(m_Buffer);
	}

	string& string::operator=(const string& other)
	{
		clear();
		append(other.c_str(), other.size());
		return *this;
	}

	string& string::operator=(string&& other) noexcept
	{
		char* thisBuffer = m_Buffer;
		m_Buffer = other.m_Buffer;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		other.m_Buffer = thisBuffer;
		return *this;
	}

	uint32_t string::size() const
	{
		return m_Size;
	}

	uint32_t string::length() const
	{
		return size();
	}

	uint32_t string::capacity() const
	{
		return m_Capacity;
	}

	const char* string::c_str() const
	{
		return buffer_ptr();
	}

	const char* string::data() const
	{
		return c_str();
	}

	std::string string::cpp_str() const
	{
		return std::string(c_str(), size());
	}

	bool string::empty() const
	{
		return size() == 0;
	}

	string::const_iterator string::begin() const noexcept
	{
		return string::const_iterator(buffer_ptr());
	}

	string::iterator string::begin() noexcept
	{
		return string::iterator(buffer_ptr());
	}

	string::const_iterator string::end() const noexcept
	{
		return string::const_iterator(buffer_ptr() + size());
	}

	string::iterator string::end() noexcept
	{
		return string::iterator(buffer_ptr() + size());
	}

	const char string::front() const
	{
		return at(0);
	}

	char& string::front()
	{
		return at(0);
	}

	const char string::back() const
	{
		return at(size() - 1);
	}

	char& string::back()
	{
		return at(size() - 1);
	}

	const char string::at(int index) const
	{
		if (index < 0)
		{
			index = size() + index;
		}
		return buffer_ptr()[index];
	}

	char& string::at(int index)
	{
		if (index < 0)
		{
			index = size() + index;
		}
		return buffer_ptr()[index];
	}

	const char string::operator[](int index) const
	{
		return at(index);
	}

	char& string::operator[](int index)
	{
		return at(index);
	}

	void string::push_back(char c)
	{
		append(c);
	}

	void string::pop_back()
	{
		erase(end() - 1);
	}

	void string::pop_front()
	{
		erase(begin());
	}

	string& string::append(const string& str)
	{
		return append(str.c_str(), str.size());
	}

	string& string::append(const char* str)
	{
		uint32_t len = calc_string_length(str);
		return append(str, len);
	}

	string& string::append(const char* str, uint32_t count)
	{
		test_size(size() + count);
		memcpy(buffer_ptr() + size(), str, count);
		m_Size += count;
		null_terminate();
		return *this;
	}

	string& string::append(char c)
	{
		return append(&c, 1);
	}

	string& string::append(const string& str, uint32_t strbegin, uint32_t strlen)
	{
		return append(str.substr(strbegin, strlen));
	}

	string& string::insert(uint32_t pos, const string& str)
	{
		return insert(pos, str.c_str(), str.size());
	}

	string& string::insert(uint32_t pos, const string& str, uint32_t strbegin, uint32_t strlen)
	{
		return insert(pos, str.substr(strbegin, strlen));
	}

	string& string::insert(uint32_t pos, const char* str)
	{
		uint32_t len = calc_string_length(str);
		return insert(pos, str, len);
	}

	string& string::insert(uint32_t pos, const char* str, uint32_t count)
	{
		shuffle_backward(pos, count);
		memcpy(buffer_ptr() + pos, str, count);
		m_Size += count;
		null_terminate();
		return *this;
	}

	string& string::insert(uint32_t pos, uint32_t count, char c)
	{
		shuffle_backward(pos, count);
		for (uint32_t i = pos; i < pos + count; i++)
		{
			buffer_ptr()[i] = c;
		}
		m_Size += count;
		null_terminate();
		return *this;
	}

	string::iterator string::insert(const_iterator pos, uint32_t count, char c)
	{
		uint32_t index = (uint32_t)(pos.get() - begin().get());
		insert(index, count, c);
		return string::iterator((char*)pos.get());
	}

	string::iterator string::insert(const_iterator pos, char c)
	{
		return insert(pos, 1, c);
	}

	void string::reserve(uint32_t new_capacity)
	{
		test_size(new_capacity - 1);
	}

	void string::clear()
	{
		m_Size = 0;
		null_terminate();
	}

	string operator+(const string& left, const string& right)
	{
		string result = left;
		result.append(right);
		return result;
	}

	string operator+(const string& left, const char* right)
	{
		string result = left;
		result.append(right);
		return result;
	}

	string operator+(const string& left, char right)
	{
		string result = left;
		result.append(right);
		return result;
	}

	string operator+(const char* left, const string& right)
	{
		string result = left;
		result.append(right);
		return result;
	}

	string operator+(char left, const string& right)
	{
		string result = left;
		result.append(right);
		return result;
	}

	string& string::operator+=(const string& other)
	{
		return append(other);
	}

	string& string::operator+=(const char* other)
	{
		return append(other);
	}

	string& string::operator+=(char other)
	{
		return append(other);
	}

	bool string::operator==(const string& other) const
	{
		return strcmp(c_str(), other.c_str()) == 0;
	}

	bool string::operator==(const char* other) const
	{
		return strcmp(c_str(), other) == 0;
	}

	bool string::operator==(char other) const
	{
		if (size() != 1)
		{
			return false;
		}
		return front() == other;
	}

	bool string::operator!=(const string& other) const
	{
		return !(*this == other);
	}

	bool string::operator!=(const char* other) const
	{
		return !(*this == other);
	}
	bool string::operator!=(char other) const
	{
		return !(*this == other);
	}

	string& string::erase(uint32_t begin, uint32_t count)
	{
		shuffle_forward(begin + count, count);
		m_Size -= count;
		null_terminate();
		return *this;
	}

	string::iterator string::erase(string::iterator chr)
	{
		return erase(chr, chr + 1);
	}

	string::iterator string::erase(string::iterator start, string::iterator finish)
	{
		uint32_t beginIndex = (uint32_t)(start.get() - begin().get());
		uint32_t len = (uint32_t)(finish.get() - start.get());
		erase(beginIndex, len);
		return start;
	}

	uint32_t string::find(const string& str, uint32_t start) const noexcept
	{
		return find(str.c_str(), str.size(), start);
	}

	uint32_t string::find(const char* str, uint32_t start) const
	{
		uint32_t len = calc_string_length(str);
		return find(str, len, start);
	}

	uint32_t string::find(const char* buff, uint32_t count, uint32_t start) const
	{
		if (start == npos)
		{
			start = size() - 1;
		}
		for (uint32_t i = start; i < size(); i++)
		{
			bool found = true;
			if (i + count > size())
			{
				break;
			}
			for (uint32_t j = 0; j < count; j++)
			{
				if (buffer_ptr()[i + j] != buff[j])
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

	uint32_t string::find(char c, uint32_t start) const noexcept
	{
		return find(&c, 1, start);
	}

	uint32_t string::rfind(const string& str, uint32_t start) const noexcept
	{
		return rfind(str.c_str(), str.size(), start);
	}

	uint32_t string::rfind(const char* str, uint32_t start) const
	{
		uint32_t len = calc_string_length(str);
		return rfind(str, len, start);
	}

	uint32_t string::rfind(const char* buff, uint32_t count, uint32_t start) const
	{
		if (start == npos)
		{
			start = size() - 1;
		}
		for (int i = start; i >= 0; i--)
		{
			bool found = true;
			if (i + count > size())
			{
				continue;
			}
			for (uint32_t j = 0; j < count; j++)
			{
				if (buffer_ptr()[i + j] != buff[j])
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

	uint32_t string::rfind(char c, uint32_t start) const noexcept
	{
		return rfind(&c, 1, start);
	}

	uint32_t string::find_first_of(const string& chars, uint32_t start) const noexcept
	{
		return find_first_of(chars.c_str(), chars.size(), start);
	}

	uint32_t string::find_first_of(const char* chars, uint32_t start) const
	{
		uint32_t len = calc_string_length(chars);
		return find_first_of(chars, len, start);
	}

	uint32_t string::find_first_of(const char* chars, uint32_t count, uint32_t start) const
	{
		for (uint32_t i = start; i < size(); i++)
		{
			for (uint32_t j = 0; j < count; j++)
			{
				if (buffer_ptr()[i] == chars[j])
				{
					return i;
				}
			}
		}
		return npos;
	}

	uint32_t string::find_first_of(char c, uint32_t start) const noexcept
	{
		return find_first_of(&c, 1, start);
	}

	uint32_t string::find_last_of(const string& chars, uint32_t start) const noexcept
	{
		return find_last_of(chars.c_str(), chars.size(), start);
	}

	uint32_t string::find_last_of(const char* chars, uint32_t start) const
	{
		uint32_t len = calc_string_length(chars);
		return find_last_of(chars, len, start);
	}

	uint32_t string::find_last_of(const char* chars, uint32_t count, uint32_t start) const
	{
		if (start == npos)
		{
			start = size() - 1;
		}
		for (int i = (int)start; i >= 0; i--)
		{
			for (int j = 0; j < (int)count; j++)
			{
				if (buffer_ptr()[i] == chars[j])
				{
					return i;
				}
			}
		}
		return npos;
	}

	uint32_t string::find_last_of(char c, uint32_t start) const noexcept
	{
		return find_last_of(&c, 1, start);
	}

	uint32_t string::find_first_not_of(const string& chars, uint32_t start) const noexcept
	{
		return find_first_not_of(chars.c_str(), chars.size(), start);
	}

	uint32_t string::find_first_not_of(const char* chars, uint32_t start) const
	{
		uint32_t len = calc_string_length(chars);
		return find_first_not_of(chars, len, start);
	}

	uint32_t string::find_first_not_of(const char* chars, uint32_t count, uint32_t start) const
	{
		for (uint32_t i = start; i < size(); i++)
		{
			bool found = false;
			for (uint32_t j = 0; j < count; j++)
			{
				if (buffer_ptr()[i] == chars[j])
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				return i;
			}
		}
		return npos;
	}

	uint32_t string::find_first_not_of(char c, uint32_t start) const noexcept
	{
		return find_first_not_of(&c, 1, start);
	}

	uint32_t string::find_last_not_of(const string& chars, uint32_t start) const noexcept
	{
		return find_last_not_of(chars.c_str(), chars.size(), start);
	}

	uint32_t string::find_last_not_of(const char* chars, uint32_t start) const
	{
		uint32_t len = calc_string_length(chars);
		return find_last_not_of(chars, len, start);
	}

	uint32_t string::find_last_not_of(const char* chars, uint32_t count, uint32_t start) const
	{
		if (start == npos)
		{
			start = size() - 1;
		}
		for (int i = (int)start; i >= 0; i--)
		{
			bool found = false;
			for (int j = 0; j < (int)count; j++)
			{
				if (buffer_ptr()[i] == chars[j])
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				return i;
			}
		}
		return npos;
	}

	uint32_t string::find_last_not_of(char c, uint32_t start) const noexcept
	{
		return find_last_not_of(&c, 1, start);
	}

	string string::substr(const_iterator start, const_iterator end) const
	{
		uint32_t beginIndex = (uint32_t)(start.get() - begin().get());
		uint32_t count = (uint32_t)(end.get() - start.get());
		return substr(beginIndex, count);
	}

	string string::substr(iterator start, iterator end) const
	{
		uint32_t beginIndex = (uint32_t)(start.get() - begin().get());
		uint32_t count = (uint32_t)(end.get() - start.get());
		return substr(beginIndex, count);
	}

	string string::substr(uint32_t start, uint32_t count) const
	{
		string result;
		if (count == npos)
		{
			count = size() - start;
		}
		result.reserve(count + 1);
		memcpy(result.buffer_ptr(), buffer_ptr() + start, count);
		result.m_Size = count;
		result.null_terminate();
		return result;
	}

	string& string::replace(uint32_t pos, uint32_t count, const string& str)
	{
		return replace(pos, count, str.c_str(), str.size());
	}

	string& string::replace(iterator i1, iterator i2, const string& str)
	{
		uint32_t pos = (uint32_t)(i1.get() - begin().get());
		uint32_t count = (uint32_t)(i2.get() - i1.get());
		return replace(pos, count, str);
	}

	string& string::replace(uint32_t pos, uint32_t count, const string& str, uint32_t strpos, uint32_t strlen)
	{
		return replace(pos, count, str.substr(strpos, strlen));
	}

	string& string::replace(uint32_t pos, uint32_t count, const char* str)
	{
		uint32_t len = calc_string_length(str);
		return replace(pos, count, str, len);
	}

	string& string::replace(iterator i1, iterator i2, const char* str)
	{
		uint32_t pos = (uint32_t)(i1.get() - begin().get());
		uint32_t count = (uint32_t)(i2.get() - i1.get());
		uint32_t len = calc_string_length(str);
		return replace(pos, count, str, len);
	}

	string& string::replace(uint32_t pos, uint32_t len, const char* str, uint32_t count)
	{
		memcpy(buffer_ptr() + pos, str, len);
		return *this;
	}

	string& string::replace(iterator i1, iterator i2, const char* str, uint32_t count)
	{
		uint32_t pos = (uint32_t)(i1.get() - begin().get());
		uint32_t len = (uint32_t)(i2.get() - i1.get());
		return replace(pos, len, str, count);
	}

	string& string::replace(uint32_t pos, uint32_t len, char c)
	{
		memset(buffer_ptr() + pos, c, len);
		return *this;
	}

	string& string::replace(iterator i1, iterator i2, char c)
	{
		uint32_t pos = (uint32_t)(i1.get() - begin().get());
		uint32_t len = (uint32_t)(i2.get() - i1.get());
		return replace(pos, len, c);
	}

	string& string::replace(iterator i1, iterator i2, iterator other1, iterator other2)
	{
		uint32_t pos = (uint32_t)(i1.get() - begin().get());
		uint32_t len = (uint32_t)(i2.get() - i1.get());
		return replace(pos, len, other1.get(), len);
	}

	string& string::replace_all(const string& replace, const string& with)
	{
		uint32_t index = find(replace);
		while (index != npos)
		{
			insert(index, with);
			erase(index + with.size(), replace.size());			
			index = find(replace, index + with.size());
		}
		return *this;
	}

	string& string::replace_all(char c, const string& with)
	{
		uint32_t index = find(c);
		while (index != npos)
		{
			erase(begin() + index, begin() + index + 1);
			insert(index, with);
			index = find(c, index + with.size());
		}
		return *this;
	}

	string& string::replace_all(const string& replace, char with)
	{
		uint32_t index = find(replace);
		while (index != npos)
		{
			erase(begin() + index, begin() + index + replace.size());
			insert(index, with);
			index = find(replace, index + 1);
		}
		return *this;
	}

	string& string::replace_all(char c, char with)
	{
		uint32_t index = find(c);
		while (index != npos)
		{
			at(index) = with;
			index = find(c, index + 1);
		}
		return *this;
	}

	string& string::remove_all(const string& chars)
	{
		return remove_all(chars.c_str());
	}

	string& string::remove_all(const char* chars)
	{
		uint32_t index = find_first_of(chars);
		while (index != npos)
		{
			erase(begin() + index);
			index = find_first_of(chars);
		}
		return *this;
	}

	string& string::remove_all(char c)
	{
		return replace_all(c, "");
	}

	bool string::contains(const string& substr, uint32_t start) const noexcept
	{
		return find(substr, start) != npos;
	}

	bool string::contains(const char* substr, uint32_t start) const
	{
		return find(substr, start) != npos;
	}

	bool string::contains(char c, uint32_t start) const noexcept
	{
		return find(c, start) != npos;
	}

	bool string::begins_with(const string& str) const noexcept
	{
		return begins_with(str.c_str());
	}

	bool string::begins_with(const char* str) const noexcept
	{
		uint32_t strlength = calc_string_length(str);
		for (uint32_t i = 0; i < strlength; i++)
		{
			if (at(i) != str[i])
			{
				return false;
			}
		}
		return true;
	}

	bool string::begins_with(char c) const noexcept
	{
		return front() == c;
	}

	bool string::ends_with(const string& str) const noexcept
	{
		return ends_with(str.c_str());
	}

	bool string::ends_with(const char* str) const noexcept
	{
		uint32_t strlength = calc_string_length(str);
		for (uint32_t i = 0; i < strlength; i++)
		{
			if (at(size() - strlength + i) != str[i])
			{
				return false;
			}
		}
		return true;
	}

	bool string::ends_with(char c) const noexcept
	{
		return back() == c;
	}

	int string::count_of(char c, uint32_t start) const noexcept
	{
		int count = 0;
		for (uint32_t i = start; i < length(); i++)
		{
			if (at(i) == c)
			{
				count++;
			}
		}
		return count;
	}

	int string::count_of(const char* str, uint32_t start) const noexcept
	{
		int count = 0;
		uint32_t len = calc_string_length(str);
		for (uint32_t i = start; i < length(); i++)
		{
			if (find(str, i) != npos)
			{
				count++;
				i += len - 1;
			}
		}
		return count;
	}

	int string::count_of(const string& str, uint32_t start) const noexcept
	{
		return count_of(str.c_str());
	}

	std::vector<string> string::split(const string& delimeter) const
	{
		return split(delimeter.c_str());
	}

	std::vector<string> string::split(const char* delimeter) const
	{
		std::vector<string> result;
		uint32_t start = 0;
		uint32_t end = find(delimeter);
		uint32_t strlength = calc_string_length(delimeter);
		while (end != npos)
		{
			result.push_back(substr(start, end - start));
			start = end + strlength;
			end = find(delimeter, start);
		}
		if (end == npos)
		{
			end = size();
		}
		result.push_back(substr(start, end - start));
		return result;
	}

	std::vector<string> string::split(const std::vector<char>& delimeters) const
	{
		std::vector<string> result;
		const char* buff = delimeters.data();
		uint32_t start = 0;
		uint32_t end = find_first_of(buff, delimeters.size(), start);
		while (end != npos)
		{
			result.push_back(substr(start, end - start));
			start = end + 1;
			end = find_first_of(buff, delimeters.size(), start);
		}
		if (end == npos)
		{
			end = size();
		}
		result.push_back(substr(start, end - start));
		return result;
	}

	std::vector<string> string::split(char delimeter) const
	{
		return split(std::vector<char>{ delimeter });
	}

	std::ostream& operator<<(std::ostream& stream, const string& str)
	{
		stream << str.c_str();
		return stream;
	}

	uint32_t string::calc_string_length(const char* str) const
	{
		uint32_t length = 0;
		while (str[length] != '\0')
		{
			length++;
		}
		return length;
	}

	void string::realloc_buffer(uint32_t new_capacity)
	{
		m_Buffer = (char*)std::realloc(m_Buffer, new_capacity);
		m_Capacity = new_capacity;
	}

	void string::test_size(uint32_t required_size)
	{
		uint32_t required_capacity = (required_size + 1) * sizeof(char);
		if (required_capacity > capacity())
		{
			realloc_buffer(required_capacity);
		}
	}

	void string::shuffle_forward(uint32_t beginIndex, uint32_t count)
	{
		if (count == 0)
		{
			return;
		}
		memmove(buffer_ptr() + beginIndex - count, buffer_ptr() + beginIndex, size() - beginIndex);
	}

	void string::shuffle_backward(uint32_t beginIndex, uint32_t count)
	{
		if (count == 0)
		{
			return;
		}
		test_size(size() + count);
		memmove(buffer_ptr() + beginIndex + count, buffer_ptr() + beginIndex, size() - beginIndex);
	}

	void string::null_terminate()
	{
		test_size(size());
		buffer_ptr()[size()] = '\0';
	}

	char* string::buffer_ptr() const
	{
		return m_Buffer;
	}

}

#endif