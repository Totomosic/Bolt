#pragma once
#include "DirectoryPath.h"

namespace Bolt
{

	struct BLT_API FilePath
	{
	private:
		blt::string m_Path;

	public:
		FilePath();
		FilePath(const blt::string& path);
		FilePath(const char* path);

		const blt::string& Path() const;
		DirectoryPath Directory() const;
		blt::string Filename() const;
		blt::string SimpleFilename() const;
		blt::string Extension() const;
		bool IsRelative() const;
		bool IsAbsolute() const;

		void MakeAbsolute(const DirectoryPath& root);
		
		friend std::ostream& operator<<(std::ostream& stream, const FilePath& path);

	public:
		static FilePath Combine(const DirectoryPath& directory, const FilePath& file);

	private:
		static void StandardizePath(blt::string& filepath);

	};

}