#pragma once
#include "FileType.h"
#include "filesystem.h"

namespace Bolt::Assets
{

	class BLT_API Texture2DConverter
	{
	public:
		static int ToBoltFormat(const std::string& assetName, const std::filesystem::path& filepath, const std::filesystem::path& output, FileType filetype);
		static int FromBoltFormat(const std::filesystem::path& filepath, const std::filesystem::path& output, FileType filetype);
	};

}