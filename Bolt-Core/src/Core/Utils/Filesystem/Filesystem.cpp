#include "Filesystem.h"
#include <experimental\filesystem>

namespace Bolt
{

	Directorypath Filesystem::s_WorkingDirectory = Directorypath();

	const Directorypath& Filesystem::WorkingDirectory()
	{
		return s_WorkingDirectory;
	}

	int Filesystem::FileSize(const Filepath& filename)
	{
		return (int)std::experimental::filesystem::file_size(filename.Path().c_str());
	}

	bool Filesystem::FileExists(const Filepath& filename)
	{
		return std::experimental::filesystem::exists(filename.Path().c_str());
	}

	void Filesystem::Rename(const Filepath& filename, const Filepath& newFilename)
	{
		std::experimental::filesystem::rename(filename.Path().c_str(), newFilename.Path().c_str());
	}

	void Filesystem::Rename(File& file, const Filepath& newFilename)
	{
		file.Rename(newFilename);
	}

	File Filesystem::GetFile(const Filepath& filename)
	{
		return File(filename);
	}

	File Filesystem::OpenFile(const Filepath& filename, OpenFlags flags)
	{
		if (flags == OpenFlags::Read)
		{
			BLT_ASSERT(FileExists(filename), "File: " + filename.Path() + " could not be found");
		}
		File f = GetFile(filename);
		f.Open(flags);
		return f;
	}

	File Filesystem::CreateNewFile(const Filepath& filename)
	{
		File f = GetFile(filename);
		f.Clear();
		return f;
	}

	void Filesystem::CreateNewDirectory(const Directorypath& directoryPath)
	{
		std::experimental::filesystem::create_directory(directoryPath.Path().c_str());
	}

	bool Filesystem::DirectoryExists(const Directorypath& path)
	{
		return std::experimental::filesystem::is_directory(path.Path().c_str());
	}

	std::vector<Directorypath> Filesystem::EnumerateDirectories(const Directorypath& path)
	{
		std::vector<Directorypath> result;
		for (auto file : std::experimental::filesystem::directory_iterator(path.Path().c_str()))
		{
			if (std::experimental::filesystem::is_directory(file))
			{
				result.emplace_back(file.path().string());
			}
		}
		return result;
	}

	std::vector<Filepath> Filesystem::EnumerateFiles(const Directorypath& path)
	{
		std::vector<Filepath> result;
		for (auto file : std::experimental::filesystem::directory_iterator(path.Path().c_str()))
		{
			if (!std::experimental::filesystem::is_directory(file))
			{
				result.emplace_back(file.path().string());
			}
		}
		return result;
	}

	bool Filesystem::Delete(const Filepath& filename)
	{
		int result = std::remove(filename.Path().c_str());
		return result == 0;
	}

	bool Filesystem::Delete(const File& file)
	{
		return Delete(file.Filename());
	}

	XMLfile Filesystem::OpenXMLFile(const Filepath& filename)
	{
		XMLfile f = XMLfile(filename);
		f.Open();
		f.Root = FileLoader::LoadXML(f);
		return f;
	}

	void Filesystem::Initialize()
	{
		char buff[FILENAME_MAX];
		BLT_GET_CURRENT_DIRECTORY(buff, FILENAME_MAX);
		s_WorkingDirectory = Directorypath(blt::string(buff) + '\\');
	}

}