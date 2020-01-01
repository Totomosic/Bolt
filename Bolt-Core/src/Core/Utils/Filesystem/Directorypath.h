#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct FilePath;

	struct BLT_API DirectoryPath
	{
	public:
		static constexpr char DIRECTORY_DELIMITER = '/';

	private:
		blt::string m_Path;

	public:
		DirectoryPath();
		DirectoryPath(const blt::string& path);
		DirectoryPath(const char* path);

		const blt::string& Path() const;
		bool HasParent() const;
		DirectoryPath Parent() const;
		bool IsRelative() const;
		bool IsAbsolute() const;

		void MakeAbsolute(const DirectoryPath& root);

		bool operator==(const DirectoryPath& other) const;
		bool operator!=(const DirectoryPath& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const DirectoryPath& path);
		friend struct FilePath;

	public:
		static DirectoryPath Combine(const DirectoryPath& left, const DirectoryPath& right);

	private:
		static void StandardizePath(blt::string& directorypath);

	};

}