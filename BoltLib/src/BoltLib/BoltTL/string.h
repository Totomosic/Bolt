#pragma once
#include <string>

#define BLT_USE_STRING 1

#if !BLT_USE_STRING

#else

#include <iostream>
#include <vector>
#include "iterator.h"

#include "basic_string.h"

namespace blt
{

	class string
	{
	public:
		using iterator = basic_string_iterator<char, false>;
		using const_iterator = basic_string_iterator<const char, false>;

	public:
		static const uint32_t npos = (uint32_t)-1;

	private:
		char* m_Buffer;
		uint32_t m_Size;
		uint32_t m_Capacity;

	public:
		string();
		explicit string(uint32_t capacity);
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

		uint32_t size() const;
		uint32_t length() const;
		uint32_t capacity() const;
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
		string& append(const char* str, uint32_t count);
		string& append(char c);
		string& append(const string& str, uint32_t strbegin, uint32_t strlen);
		string& insert(uint32_t pos, const string& str);
		string& insert(uint32_t pos, const string& str, uint32_t strbegin, uint32_t strlen = npos);
		string& insert(uint32_t pos, const char* str);
		string& insert(uint32_t pos, const char* str, uint32_t count);
		string& insert(uint32_t pos, uint32_t count, char c);
		iterator insert(const_iterator pos, uint32_t count, char c);
		iterator insert(const_iterator pos, char c);

		void reserve(uint32_t new_capacity);
		void resize(uint32_t n, char c = {});
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

		string& erase(uint32_t begin, uint32_t count);
		iterator erase(iterator character);
		iterator erase(iterator start, iterator finish);
		uint32_t find(const string& str, uint32_t start = 0) const noexcept;
		uint32_t find(const char* str, uint32_t start = 0) const;
		uint32_t find(const char* buff, uint32_t count, uint32_t start) const;
		uint32_t find(char c, uint32_t start = 0) const noexcept;
		uint32_t rfind(const string& str, uint32_t start = npos) const noexcept;
		uint32_t rfind(const char* str, uint32_t start = npos) const;
		uint32_t rfind(const char* buff, uint32_t count, uint32_t start) const;
		uint32_t rfind(char c, uint32_t start = npos) const noexcept;
		uint32_t find_first_of(const string& chars, uint32_t start = 0) const noexcept;
		uint32_t find_first_of(const char* chars, uint32_t start = 0) const;
		uint32_t find_first_of(const char* chars, uint32_t count, uint32_t start) const;
		uint32_t find_first_of(char c, uint32_t start = 0) const noexcept;
		uint32_t find_last_of(const string& chars, uint32_t start = npos) const noexcept;
		uint32_t find_last_of(const char* chars, uint32_t start = npos) const;
		uint32_t find_last_of(const char* chars, uint32_t count, uint32_t start) const;
		uint32_t find_last_of(char c, uint32_t start = npos) const noexcept;
		uint32_t find_first_not_of(const string& chars, uint32_t start = 0) const noexcept;
		uint32_t find_first_not_of(const char* chars, uint32_t start = 0) const;
		uint32_t find_first_not_of(const char* chars, uint32_t count, uint32_t start) const;
		uint32_t find_first_not_of(char c, uint32_t start = 0) const noexcept;
		uint32_t find_last_not_of(const string& chars, uint32_t start = npos) const noexcept;
		uint32_t find_last_not_of(const char* chars, uint32_t start = npos) const;
		uint32_t find_last_not_of(const char* chars, uint32_t count, uint32_t start) const;
		uint32_t find_last_not_of(char c, uint32_t start = npos) const noexcept;

		string substr(const_iterator start, const_iterator end) const;
		string substr(iterator start, iterator end) const;
		string substr(uint32_t start, uint32_t count = npos) const;

		string& replace(uint32_t pos, uint32_t count, const string& str);
		string& replace(iterator i1, iterator i2, const string& str);
		string& replace(uint32_t pos, uint32_t len, const string& str, uint32_t strpos, uint32_t strlen);
		string& replace(uint32_t pos, uint32_t len, const char* str);
		string& replace(iterator i1, iterator i2, const char* str);
		string& replace(uint32_t pos, uint32_t len, const char* buff, uint32_t count);
		string& replace(iterator i1, iterator i2, const char* buff, uint32_t count);
		string& replace(uint32_t pos, uint32_t len, char c);
		string& replace(iterator i1, iterator i2, char c);
		string& replace(iterator i1, iterator i2, iterator other1, iterator other2);

		string& replace_all(const string& replace, const string& with);
		string& replace_all(char c, const string& with);
		string& replace_all(const string& replace, char with);
		string& replace_all(char c, char with);

		string& remove_all(const string& chars);
		string& remove_all(const char* chars);
		string& remove_all(char c);

		bool contains(const string& str, uint32_t start = 0) const noexcept;
		bool contains(const char* str, uint32_t start = 0) const;
		bool contains(char c, uint32_t start = 0) const noexcept;

		bool begins_with(const string& str) const noexcept;
		bool begins_with(const char* str) const noexcept;
		bool begins_with(char c) const noexcept;
		bool ends_with(const string& str) const noexcept;
		bool ends_with(const char* str) const noexcept;
		bool ends_with(char c) const noexcept;

		int count_of(char c, uint32_t start = 0) const noexcept;
		int count_of(const string& str, uint32_t start = 0) const noexcept;
		int count_of(const char* str, uint32_t start = 0) const noexcept;

		std::vector<string> split(const string& delimeter) const;
		std::vector<string> split(const char* delimeter) const;
		std::vector<string> split(const std::vector<char>& delimeters) const;
		std::vector<string> split(char delimeter) const;

		friend std::ostream& operator<<(std::ostream& stream, const string& str);

	private:
		uint32_t calc_string_length(const char* str) const;
		void realloc_buffer(uint32_t new_capacity);
		void test_size(uint32_t required_size);
		void shuffle_forward(uint32_t beginIndex, uint32_t count);
		void shuffle_backward(uint32_t beginIndex, uint32_t count);
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
			return hash<std::string>()(str.cpp_str());
		}
	};

}

#endif