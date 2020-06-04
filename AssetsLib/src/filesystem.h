#pragma once
#ifdef BLT_PLATFORM_WINDOWS
#include <filesystem>
#else
#include <experimental/filesystem>
namespace std
{
	namespace filesystem = std::experimental::filesystem;
}
#endif