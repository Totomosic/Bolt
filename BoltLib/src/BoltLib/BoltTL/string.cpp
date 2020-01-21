#include "string.h"

namespace blt
{

	std::vector<std::string_view> split_view(const std::string_view& string, const std::string& delimiter)
	{
		std::string_view s = string;
		std::vector<std::string_view> result;
		size_t pos = 0;
		std::string_view token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			result.push_back(token);
			s = s.substr(pos + delimiter.length());
		}
		result.push_back(s);
		return result;
	}

	std::vector<std::string_view> split_view(const std::string_view& string, const char* delimiter)
	{
		return split_view(string, std::string(delimiter));
	}

	std::vector<std::string_view> split_view(const std::string_view& string, char delimiter)
	{
		return split_view(string, std::string({ delimiter }));
	}

	std::vector<std::string> split(const std::string& string, const std::string& delimiter)
	{
		std::string s = string;
		std::vector<std::string> result;
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			result.push_back(token);
			s = s.substr(pos + delimiter.length());
		}
		result.push_back(s);
		return result;
	}

	std::vector<std::string> split(const std::string& string, const char* delimiter)
	{
		return split(string, std::string(delimiter));
	}

	std::vector<std::string> split(const std::string& string, char delimiter)
	{
		return split(string, std::string({ delimiter }));
	}

	std::string& replace_all(std::string& string, char replace, char with)
	{
		for (char& c : string)
		{
			if (c == replace)
				c = with;
		}
		return string;
	}

	std::string& remove_all(std::string& string, const std::string& chars, typename std::string::size_type start)
	{
		for (char chr : chars)
		{
			size_t current = start;
			while ((current = string.find(chr, current)) != std::string::npos)
			{
				string.erase(string.begin() + current);
			}
		}
		return string;
	}

	std::string& remove_all(std::string& string, const char* chars, typename std::string::size_type start)
	{
		return remove_all(string, std::string(chars), start);
	}

	std::string& remove_all(std::string& string, char chr, typename std::string::size_type start)
	{
		return remove_all(string, std::string({ chr }), start);
	}

}