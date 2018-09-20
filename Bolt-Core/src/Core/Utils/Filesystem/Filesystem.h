#pragma once
#include "Directorypath.h"
#include "File.h"
#include "Filepath.h"
#include "FileLoader.h"

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

		friend class Engine;

	private:
		static void Initialize();

	};

}