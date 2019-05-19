#include "bltpch.h"

#include "Directorypath.h"
#include "Filesystem.h"
#include "Filepath.h"

namespace Bolt
{

	Directorypath::Directorypath()
		: m_Path()
	{
		
	}

	Directorypath::Directorypath(const blt::string& path)
		: m_Path(path)
	{
		StandardizePath(m_Path);
	}

	Directorypath::Directorypath(const char* path) : Directorypath(blt::string(path))
	{
	
	}

	const blt::string& Directorypath::Path() const
	{
		return m_Path;
	}

	bool Directorypath::HasParent() const
	{
		return !Parent().Path().empty();
	}

	Directorypath Directorypath::Parent() const
	{
		uint32_t index = m_Path.find_last_of('\\', m_Path.length() - 2);
		return m_Path.substr(0, index);
	}

	bool Directorypath::IsRelative() const
	{
		return !IsAbsolute();
	}

	bool Directorypath::IsAbsolute() const
	{
		return m_Path.size() > 1 && m_Path[1] == ':';
	}

	void Directorypath::MakeAbsolute(const Directorypath& root)
	{
		if (IsRelative())
		{
			m_Path = Directorypath::Combine(root, *this).Path();
		}
	}

	bool Directorypath::operator==(const Directorypath& other) const
	{
		return other.Path() == Path();
	}

	bool Directorypath::operator!=(const Directorypath& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const Directorypath& path)
	{
		stream << path.Path();
		return stream;
	}

	Directorypath Directorypath::Combine(const Directorypath& left, const Directorypath& right)
	{
		BLT_ASSERT(!(left.IsAbsolute() && right.IsAbsolute()), "Unable to combine 2 absolute paths");
		BLT_ASSERT(right.IsRelative(), "Right path must be relative in order to combine");
		blt::string leftPath = left.Path();
		blt::string rightPath = right.Path();
		return Directorypath(leftPath + rightPath);
	}

	void Directorypath::StandardizePath(blt::string& directorypath)
	{
		directorypath.replace_all('/', '\\');
		if (directorypath.front() == '\\')
		{
			directorypath = directorypath.substr(1);
		}
		if (directorypath.back() != '\\')
		{
			directorypath.append('\\');
		}
	}

}