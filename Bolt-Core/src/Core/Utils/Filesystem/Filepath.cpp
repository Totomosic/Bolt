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
		if (index != std::string::npos)
		{
			DirectoryPath directory = Path().substr(0, index);
			return directory;
		}
		return "";
	}

	std::string FilePath::Filename() const
	{
		size_t index = m_Path.find_last_of(DirectoryPath::DIRECTORY_DELIMITER);
		if (index != std::string::npos)
		{
			return m_Path.substr(index + 1, m_Path.length() - index - 1);
		}
		return m_Path;
	}

	std::string FilePath::SimpleFilename() const
	{
		size_t index = m_Path.find_last_of(DirectoryPath::DIRECTORY_DELIMITER);
		std::string_view path = m_Path;
		if (index != std::string::npos)
		{
			path = path.substr(index + 1);
		}
		size_t extIndex = path.find_last_of('.');
		if (extIndex != std::string_view::npos)
		{
			return std::string(path.substr(index + 1, index - extIndex - 1));
		}
		return std::string(path);
	}
	
	std::string FilePath::Extension() const
	{
		size_t index = m_Path.find_last_of('.');
		if (index != std::string::npos)
		{
			return m_Path.substr(index + 1, m_Path.length() - index - 1);
		}
		return "";
	}

	std::ostream& operator<<(std::ostream& stream, const FilePath& path)
	{
		stream << path.Path();
		return stream;
	}

	FilePath FilePath::Combine(const DirectoryPath& directory, const FilePath& file)
	{
		const std::string& leftPath = directory.Path();
		const std::string& rightPath = file.Path();
		if (leftPath.back() == DirectoryPath::DIRECTORY_DELIMITER || rightPath.front() == DirectoryPath::DIRECTORY_DELIMITER)
		{
			return FilePath(leftPath + rightPath);
		}
		return FilePath(leftPath + DirectoryPath::DIRECTORY_DELIMITER + rightPath);
	}

	void FilePath::StandardizePath(std::string& filepath)
	{
		blt::replace_all(filepath, '\\', DirectoryPath::DIRECTORY_DELIMITER);
	}

}