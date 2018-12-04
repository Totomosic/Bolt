#pragma once
#include <string>

#define BLT_USE_STRING 1

#if !BLT_USE_STRING

namespace blt
{
	typedef std::string string;
}

#else

#include <iostream>
#include <vector>
#include "iterator.h"

namespace blt
{

	class string
	{
	public:
		typedef blt::iterator<char> iterator;
		typedef blt::iterator<const char> const_iterator;

	public:
		static const size_t npos = (size_t)-1;

	private:
		char* m_Buffer;
		size_t m_Size;
		size_t m_Capacity;

	public:
		string();
		string(size_t capacity);
		string(char c);
		string(const char* str);
		string(const char* buffer, unsigned int size);
		string(const std::string& str);
		string(std::initializer_list<char> il);
		string(const string& other);
		string(string&& other) noexcept;
		~string();

		string& operator=(const string& other);
		string& operator=(string&& other) noexcept;

		size_t size() const;
		size_t length() const;
		size_t capacity() const;
		const char* c_str() const;
		const char* data() const;
		std::string cpp_str() const;
		bool empty() const;

		const_iterator begin() const noexcept;
		iterator begin() noexcept;
		const_iterator end() const noexcept;
		iterator end() noexcept;

		const char front() const;
		char& front();
		const char back() const;
		char& back();

		const char at(int index) const;
		char& at(int index);
		const char operator[](int index) const;
		char& operator[](int index);

		void push_back(char c);
		void pop_back();
		void pop_front();

		string& append(const string& str);
		string& append(const char* str);
		string& append(const char* str, size_t count);
		string& append(char c);
		string& append(const string& str, size_t strbegin, size_t strlen);
		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const string& str, size_t strbegin, size_t strlen = npos);
		string& insert(size_t pos, const char* str);
		string& insert(size_t pos, const char* str, size_t count);
		string& insert(size_t pos, size_t count, char c);
		iterator insert(const_iterator pos, size_t count, char c);
		iterator insert(const_iterator pos, char c);

		void reserve(size_t new_capacity);
		void clear();

		friend string operator+(const string& left, const string& right);
		friend string operator+(const string& left, const char* right);
		friend string operator+(const string& left, char right);
		friend string operator+(const char* left, const string& right);
		friend string operator+(char left, const string& right);

		string& operator+=(const string& other);
		string& operator+=(const char* other);
		string& operator+=(char other);

		bool operator==(const string& other) const;
		bool operator==(const char* other) const;
		bool operator==(char other) const;
		bool operator!=(const string& other) const;
		bool operator!=(const char* other) const;
		bool operator!=(char other) const;

		string& erase(size_t begin, size_t count);
		iterator erase(iterator character);
		iterator erase(iterator start, iterator finish);
		size_t find(const string& str, size_t start = 0) const noexcept;
		size_t find(const char* str, size_t start = 0) const;
		size_t find(const char* buff, size_t count, size_t start) const;
		size_t find(char c, size_t start = 0) const noexcept;
		size_t rfind(const string& str, size_t start = npos) const noexcept;
		size_t rfind(const char* str, size_t start = npos) const;
		size_t rfind(const char* buff, size_t count, size_t start) const;
		size_t rfind(char c, size_t start = npos) const noexcept;
		size_t find_first_of(const string& chars, size_t start = 0) const noexcept;
		size_t find_first_of(const char* chars, size_t start = 0) const;
		size_t find_first_of(const char* chars, size_t count, size_t start) const;
		size_t find_first_of(char c, size_t start = 0) const noexcept;
		size_t find_last_of(const string& chars, size_t start = npos) const noexcept;
		size_t find_last_of(const char* chars, size_t start = npos) const;
		size_t find_last_of(const char* chars, size_t count, size_t start) const;
		size_t find_last_of(char c, size_t start = npos) const noexcept;
		size_t find_first_not_of(const string& chars, size_t start = 0) const noexcept;
		size_t find_first_not_of(const char* chars, size_t start = 0) const;
		size_t find_first_not_of(const char* chars, size_t count, size_t start) const;
		size_t find_first_not_of(char c, size_t start = 0) const noexcept;
		size_t find_last_not_of(const string& chars, size_t start = npos) const noexcept;
		size_t find_last_not_of(const char* chars, size_t start = npos) const;
		size_t find_last_not_of(const char* chars, size_t count, size_t start) const;
		size_t find_last_not_of(char c, size_t start = npos) const noexcept;

		string substr(const_iterator start, const_iterator end) const;
		string substr(iterator start, iterator end) const;
		string substr(size_t start, size_t count = npos) const;

		string& replace(size_t pos, size_t count, const string& str);
		string& replace(iterator i1, iterator i2, const string& str);
		string& replace(size_t pos, size_t len, const string& str, size_t strpos, size_t strlen);
		string& replace(size_t pos, size_t len, const char* str);
		string& replace(iterator i1, iterator i2, const char* str);
		string& replace(size_t pos, size_t len, const char* buff, size_t count);
		string& replace(iterator i1, iterator i2, const char* buff, size_t count);
		string& replace(size_t pos, size_t len, char c);
		string& replace(iterator i1, iterator i2, char c);
		string& replace(iterator i1, iterator i2, iterator other1, iterator other2);

		string& replace_all(const string& replace, const string& with);
		string& replace_all(char c, const string& with);
		string& replace_all(const string& replace, char with);
		string& replace_all(char c, char with);

		string& remove_all(const string& chars);
		string& remove_all(const char* chars);
		string& remove_all(char c);

		bool contains(const string& str, size_t start = 0) const noexcept;
		bool contains(const char* str, size_t start = 0) const;
		bool contains(char c, size_t start = 0) const noexcept;

		bool begins_with(const string& str) const noexcept;
		bool begins_with(const char* str) const noexcept;
		bool begins_with(char c) const noexcept;
		bool ends_with(const string& str) const noexcept;
		bool ends_with(const char* str) const noexcept;
		bool ends_with(char c) const noexcept;

		int count_of(char c, size_t start = 0) const noexcept;
		int count_of(const string& str, size_t start = 0) const noexcept;
		int count_of(const char* str, size_t start = 0) const noexcept;

		std::vector<string> split(const string& delimeter) const;
		std::vector<string> split(const char* delimeter) const;
		std::vector<string> split(const std::vector<char>& delimeters) const;
		std::vector<string> split(char delimeter) const;

		friend std::ostream& operator<<(std::ostream& stream, const string& str);

	private:
		size_t calc_string_length(const char* str) const;
		void realloc_buffer(size_t new_capacity);
		void test_size(size_t required_size);
		void shuffle_forward(size_t beginIndex, size_t count);
		void shuffle_backward(size_t beginIndex, size_t count);
		void null_terminate();

		char* buffer_ptr() const;

	};

}

namespace std
{

	template<>
	struct hash<blt::string>
	{
		size_t operator()(const blt::string& str) const noexcept
		{
			return 42;
		}
	};

}

#endif