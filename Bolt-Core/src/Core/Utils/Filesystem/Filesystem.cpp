#include "bltpch.h"

#include "Filesystem.h"
#include <filesystem>
#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	DirectoryPath Filesystem::s_WorkingDirectory = DirectoryPath();

	const DirectoryPath& Filesystem::WorkingDirectory()
	{
		return s_WorkingDirectory;
	}

	bool Filesystem::FileExists(const FilePath& filepath)
	{
		return std::filesystem::exists(filepath.Path().c_str());
	}

	File Filesystem::Open(const FilePath& filepath, OpenMode mode)
	{
		BLT_ASSERT(mode != OpenMode::Read || FileExists(filepath), "No file with path " + filepath.Path());
		File f(filepath);
		f.SetOpenMode(mode);
		f.m_Stream.open(filepath.Path().c_str(), f.FlagsToValue(mode));
		if (f.m_Stream.fail())
		{
			BLT_CORE_ERROR("Failed to open file {}", filepath.Path());
			BLT_CORE_ERROR(std::strerror(errno));
		}
		return f;
	}

	void Filesystem::Close(File& file)
	{
		file.SetOpenMode(OpenMode::None);
		file.m_Stream.close();
	}

	XMLfile Filesystem::OpenXML(const FilePath& filepath, OpenMode mode)
	{
		BLT_ASSERT(mode != OpenMode::Read || FileExists(filepath), "No file with path " + filepath.Path());
		XMLfile f(filepath);
		f.SetOpenMode(mode);
		f.m_Stream.open(filepath.Path().c_str(), f.FlagsToValue(mode));
		return f;
	}

	void Filesystem::WatchFile(const FilePath& filepath, std::function<bool(const FilePath&)> callback)
	{
		
	}

	void Filesystem::Initialize()
	{
		BLT_PROFILE_FUNCTION();
		char buff[FILENAME_MAX];
		auto result = BLT_GET_CURRENT_DIRECTORY(buff, FILENAME_MAX);
		s_WorkingDirectory = DirectoryPath(blt::string(buff) + DirectoryPath::DIRECTORY_DELIMITER);
	}

}