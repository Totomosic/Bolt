#pragma once
#include "DirectoryPath.h"
#include "File.h"
#include "FilePath.h"
#include "XML/XMLfile.h"

namespace Bolt
{

	class BLT_API Filesystem
	{
	public:
		Filesystem() = delete;

		static bool FileExists(const FilePath& filepath);

		static File Open(const FilePath& filepath, OpenMode mode = OpenMode::Read);
		static void Close(File& file);
		static XMLfile OpenXML(const FilePath& filepath, OpenMode mode = OpenMode::Read);

		// Watches for change in filename or updated file data, callback returns true if should keep watching the file
		static void WatchFile(const FilePath& filepath, std::function<bool(const FilePath&)> callback);

		friend class Initializer;

	private:
		static void Initialize();

	};

}