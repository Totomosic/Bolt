#include "Types.h"

#include "Filesystem.h"
#include <experimental\filesystem>

namespace Bolt
{

	Directorypath Filesystem::s_WorkingDirectory = Directorypath();

	const Directorypath& Filesystem::WorkingDirectory()
	{
		return s_WorkingDirectory;
	}

	bool Filesystem::FileExists(const Filepath& filepath)
	{
		std::ifstream f(filepath.Path().c_str());
		return f.good();
	}

	File Filesystem::Open(const Filepath& filepath, OpenMode mode)
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

	XMLfile Filesystem::OpenXML(const Filepath& filepath, OpenMode mode)
	{
		BLT_ASSERT(mode != OpenMode::Read || FileExists(filepath), "No file with path " + filepath.Path());
		XMLfile f(filepath);
		f.SetOpenMode(mode);
		f.m_Stream.open(filepath.Path().c_str(), f.FlagsToValue(mode));
		return f;
	}

	void Filesystem::WatchFile(const Filepath& filepath, std::function<bool(const Filepath&)> callback)
	{
		
	}

	void Filesystem::Initialize()
	{
		char buff[FILENAME_MAX];
		BLT_GET_CURRENT_DIRECTORY(buff, FILENAME_MAX);
		s_WorkingDirectory = Directorypath(blt::string(buff) + '\\');
	}

}