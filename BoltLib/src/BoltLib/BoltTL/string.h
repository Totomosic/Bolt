#pragma once
#include <string>

#include <iostream>
#include <vector>
#include "iterator.h"

#include "basic_string.h"

namespace blt
{

	std::vector<std::string_view> split_view(const std::string_view& string, const std::string& delimiter);
	std::vector<std::string_view> split_view(const std::string_view& string, const char* delimiter);
	std::vector<std::string_view> split_view(const std::string_view& string, char delimiter);

	std::vector<std::string> split(const std::string& string, const std::string& delimiter);
	std::vector<std::string> split(const std::string& string, const char* delimiter);
	std::vector<std::string> split(const std::string& string, char delimiter);

	std::string& replace_all(std::string& string, char replace, char with);

	std::string& remove_all(std::string& string, const std::string& chars, typename std::string::size_type start = 0);
	std::string& remove_all(std::string& string, const char* chars, typename std::string::size_type start = 0);
	std::string& remove_all(std::string& string, char chr, typename std::string::size_type start = 0);

}