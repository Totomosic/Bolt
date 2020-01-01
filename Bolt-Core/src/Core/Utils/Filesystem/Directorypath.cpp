#include "bltpch.h"

#include "DirectoryPath.h"
#include "Filesystem.h"
#include "FilePath.h"

namespace Bolt
{

	DirectoryPath::DirectoryPath()
		: m_Path()
	{
		
	}

	DirectoryPath::DirectoryPath(const blt::string& path)
		: m_Path(path)
	{
		StandardizePath(m_Path);
	}

	DirectoryPath::DirectoryPath(const char* path) : DirectoryPath(blt::string(path))
	{
	
	}

	const blt::string& DirectoryPath::Path() const
	{
		return m_Path;
	}

	bool DirectoryPath::HasParent() const
	{
		return !Parent().Path().empty();
	}

	DirectoryPath DirectoryPath::Parent() const
	{
		uint32_t index = m_Path.find_last_of(DIRECTORY_DELIMITER, m_Path.length() - 2);
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
		blt::string leftPath = left.Path();
		blt::string rightPath = right.Path();
		return DirectoryPath(leftPath + rightPath);
	}

	void DirectoryPath::StandardizePath(blt::string& directorypath)
	{
		directorypath.replace_all('\\', DIRECTORY_DELIMITER);
		if (directorypath.front() == DIRECTORY_DELIMITER)
		{
			directorypath = directorypath.substr(1);
		}
		if (directorypath.back() != DIRECTORY_DELIMITER)
		{
			directorypath.append(DIRECTORY_DELIMITER);
		}
	}

}