#include "bltpch.h"

#include "Directorypath.h"
#include "Filesystem.h"
#include "Filepath.h"

namespace Bolt
{

	DirectoryPath::DirectoryPath()
		: m_Path()
	{
		
	}

	DirectoryPath::DirectoryPath(const std::string& path)
		: m_Path(path)
	{
		StandardizePath(m_Path);
	}

	DirectoryPath::DirectoryPath(const char* path) : DirectoryPath(std::string(path))
	{
	
	}

	const std::string& DirectoryPath::Path() const
	{
		return m_Path;
	}

	bool DirectoryPath::HasParent() const
	{
		return !Parent().Path().empty();
	}

	DirectoryPath DirectoryPath::Parent() const
	{
		size_t index = m_Path.find_last_of(DIRECTORY_DELIMITER, m_Path.length() - 2);
		if (index != std::string::npos)
		{
			return m_Path.substr(0, index);
		}
		return "";
	}

	bool DirectoryPath::operator==(const DirectoryPath& other) const
	{
		return other.Path() == Path();
	}

	bool DirectoryPath::operator!=(const DirectoryPath& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const DirectoryPath& path)
	{
		stream << path.Path();
		return stream;
	}

	DirectoryPath DirectoryPath::Combine(const DirectoryPath& left, const DirectoryPath& right)
	{
		const std::string& leftPath = left.Path();
		const std::string& rightPath = right.Path();
		if (leftPath.size() == 0)
			return DirectoryPath(rightPath);
		if (rightPath.size() == 0)
			return DirectoryPath(leftPath);
		if (leftPath.back() == DIRECTORY_DELIMITER || rightPath.front() == DIRECTORY_DELIMITER)
		{
			return DirectoryPath(leftPath + rightPath);
		}
		return DirectoryPath(leftPath + DIRECTORY_DELIMITER + rightPath);
	}

	void DirectoryPath::StandardizePath(std::string& directorypath)
	{
		blt::replace_all(directorypath, '\\', DIRECTORY_DELIMITER);
	}

}