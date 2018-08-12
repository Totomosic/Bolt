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

		static int FileSize(const Filepath& filename);
		static bool FileExists(const Filepath& filename);

		static void Rename(const Filepath& filename, const Filepath& newFilename);
		static void Rename(File& file, const Filepath& newFilename);

		static File GetFile(const Filepath& filename);
		static File OpenFile(const Filepath& filename, OpenFlags flags = OpenFlags::Read);
		static File CreateNewFile(const Filepath& filename);
		static void CreateNewDirectory(const Directorypath& directoryPath);
		static bool DirectoryExists(const Directorypath& path);

		static std::vector<Directorypath> EnumerateDirectories(const Directorypath& path);
		static std::vector<Filepath> EnumerateFiles(const Directorypath& path);
		
		static bool Delete(const Filepath& filename);
		static bool Delete(const File& file);

		static XMLfile OpenXMLFile(const Filepath& filename);

		friend class Engine;

	private:
		static void Initialize();

	};

}