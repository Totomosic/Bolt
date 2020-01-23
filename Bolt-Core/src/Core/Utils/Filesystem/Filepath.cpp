#include "bltpch.h"

#include "Filepath.h"
#include "Filesystem.h"
#include "Directorypath.h"

namespace Bolt
{

	FilePath::FilePath()
		: m_Path()
	{

	}

	FilePath::FilePath(const std::string& path)
		: m_Path(path)
	{
		StandardizePath(m_Path);
	}

	FilePath::FilePath(const char* path) : FilePath(std::string(path))
	{

	}

	const std::string& FilePath::Path() const
	{
		return m_Path;
	}

	DirectoryPath FilePath::Directory() const
	{
		size_t index = m_Path.find_last_of(DirectoryPath::DIRECTORY_DELIMITER);
		DirectoryPath directory = Path().substr(0, index);
		return directory;
	}

	std::string FilePath::Filename() const
	{
		size_t index = m_Path.find_last_of(DirectoryPath::DIRECTORY_DELIMITER);
		return m_Path.substr(index + 1, m_Path.length() - index - 1);
	}

	std::string FilePath::SimpleFilename() const
	{
		size_t index = m_Path.find_last_of(DirectoryPath::DIRECTORY_DELIMITER);
		size_t extIndex = m_Path.find_last_of('.');
		return m_Path.substr(index + 1, index - extIndex - 1);
	}
	
	std::string FilePath::Extension() const
	{
		size_t index = m_Path.find_last_of('.');
		return m_Path.substr(index + 1, m_Path.length() - index - 1);
	}

	bool FilePath::IsRelative() const
	{
		return !IsAbsolute();
	}

	bool FilePath::IsAbsolute() const
	{
		return m_Path.size() > 1 && m_Path[1] == ':';
	}

	void FilePath::MakeAbsolute(const DirectoryPath& root)
	{
		if (IsRelative())
		{
			m_Path = FilePath::Combine(root, *this).Path();
		}
	}

	std::ostream& operator<<(std::ostream& stream, const FilePath& path)
	{
		stream << path.Path();
		return stream;
	}

	FilePath FilePath::Combine(const DirectoryPath& directory, const FilePath& file)
	{
		BLT_ASSERT(!(directory.IsAbsolute() && file.IsAbsolute()), "Unable to combine 2 absolute paths");
		BLT_ASSERT(file.IsRelative(), "Right path must be relative in order to combine");
		std::string leftPath = directory.Path();
		std::string rightPath = file.Path();
		return FilePath(leftPath + rightPath);
	}

	void FilePath::StandardizePath(std::string& filepath)
	{
		blt::replace_all(filepath, '\\', DirectoryPath::DIRECTORY_DELIMITER);
	}

}