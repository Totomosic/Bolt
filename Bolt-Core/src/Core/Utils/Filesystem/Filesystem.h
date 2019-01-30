#pragma once
#include "Directorypath.h"
#include "File.h"
#include "Filepath.h"
#include "XML\__XML__.h"

#define BLT_GET_CURRENT_DIRECTORY _getcwd

namespace Bolt
{

	class BLT_API Filesystem
	{
	private:
		static Directorypath s_WorkingDirectory;

	public:
		Filesystem() = delete;

		static const Directorypath& WorkingDirectory();

		static bool FileExists(const Filepath& filepath);

		static File Open(const Filepath& filepath, OpenMode mode = OpenMode::Read);
		static void Close(File& file);
		static XMLfile OpenXML(const Filepath& filepath, OpenMode mode = OpenMode::Read);

		friend class Initializer;

	private:
		static void Initialize();

	};

}