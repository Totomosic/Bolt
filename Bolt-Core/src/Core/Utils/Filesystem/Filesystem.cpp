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
		File f;
		f.SetOpenMode(mode);
		f.m_Stream.open(filepath.Path().c_str(), f.FlagsToValue(mode));
		return f;
	}

	void Filesystem::Close(File& file)
	{
		file.SetOpenMode(OpenMode::None);
		file.m_Stream.close();
	}

	void Filesystem::Initialize()
	{
		char buff[FILENAME_MAX];
		BLT_GET_CURRENT_DIRECTORY(buff, FILENAME_MAX);
		s_WorkingDirectory = Directorypath(blt::string(buff) + '\\');
	}

}