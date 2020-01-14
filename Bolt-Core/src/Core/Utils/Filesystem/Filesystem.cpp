#include "bltpch.h"

#include "Filesystem.h"
#ifdef BLT_PLATFORM_WINDOWS
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	DirectoryPath Filesystem::s_WorkingDirectory = DirectoryPath();

	const DirectoryPath& Filesystem::WorkingDirectory()
	{
		return s_WorkingDirectory;
	}

	bool Filesystem::FileExists(const FilePath& filepath)
	{
#ifdef BLT_PLATFORM_WINDOWS
		return std::filesystem::exists(filepath.Path().c_str());
#else
		return std::experimental::filesystem::exists(filepath.Path().c_str());
#endif
	}

	File Filesystem::Open(const FilePath& filepath, OpenMode mode)
	{
		BLT_ASSERT(mode != OpenMode::Read || FileExists(filepath), "No file with path " + filepath.Path());
		File f(filepath);
		f.SetOpenMode(mode);
		f.m_Stream.open(filepath.Path().c_str(), (std::ios_base::openmode)f.FlagsToValue(mode));
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
		f.m_Stream.open(filepath.Path().c_str(), (std::ios_base::openmode)f.FlagsToValue(mode));
		return f;
	}

	void Filesystem::WatchFile(const FilePath& filepath, std::function<bool(const FilePath&)> callback)
	{
		
	}

	void Filesystem::Initialize()
	{
		#ifdef BLT_PLATFORM_WINDOWS
		BLT_PROFILE_FUNCTION();
		char buff[FILENAME_MAX];
		auto result = BLT_GET_CURRENT_DIRECTORY(buff, FILENAME_MAX);
		s_WorkingDirectory = DirectoryPath(std::string(buff) + DirectoryPath::DIRECTORY_DELIMITER);
		#endif
	}

}