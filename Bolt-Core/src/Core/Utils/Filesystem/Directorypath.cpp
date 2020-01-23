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
		return m_Path.substr(0, index);
	}

	bool DirectoryPath::IsRelative() const
	{
		return !IsAbsolute();
	}

	bool DirectoryPath::IsAbsolute() const
	{
		return m_Path.size() > 1 && m_Path[1] == ':';
	}

	void DirectoryPath::MakeAbsolute(const DirectoryPath& root)
	{
		if (IsRelative())
		{
			m_Path = DirectoryPath::Combine(root, *this).Path();
		}
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
		BLT_ASSERT(!(left.IsAbsolute() && right.IsAbsolute()), "Unable to combine 2 absolute paths");
		BLT_ASSERT(right.IsRelative(), "Right path must be relative in order to combine");
		std::string leftPath = left.Path();
		std::string rightPath = right.Path();
		return DirectoryPath(leftPath + rightPath);
	}

	void DirectoryPath::StandardizePath(std::string& directorypath)
	{
		blt::replace_all(directorypath, '\\', DIRECTORY_DELIMITER);
		if (directorypath.front() == DIRECTORY_DELIMITER)
		{
			directorypath.erase(directorypath.begin());
		}
		if (directorypath.back() != DIRECTORY_DELIMITER)
		{
			directorypath += DIRECTORY_DELIMITER;
		}
	}

}