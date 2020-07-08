#pragma once
#include "Directorypath.h"

namespace Bolt
{

	struct BLT_API FilePath
	{
	private:
		std::string m_Path;

	public:
		FilePath();
		FilePath(const std::string& path);
		FilePath(const char* path);

		const std::string& Path() const;
		DirectoryPath Directory() const;
		std::string Filename() const;
		std::string SimpleFilename() const;
		std::string Extension() const;
		
		friend std::ostream& operator<<(std::ostream& stream, const FilePath& path);

	public:
		static FilePath Combine(const DirectoryPath& directory, const FilePath& file);

	private:
		static void StandardizePath(std::string& filepath);

	};

}