#include "bltpch.h"

#include "Filepath.h"
#include "Filesystem.h"
#include "Directorypath.h"

namespace Bolt
{

	Filepath::Filepath()
		: m_Path()
	{

	}

	Filepath::Filepath(const blt::string& path)
		: m_Path(path)
	{
		StandardizePath(m_Path);
	}

	Filepath::Filepath(const char* path) : Filepath(blt::string(path))
	{

	}

	const blt::string& Filepath::Path() const
	{
		return m_Path;
	}

	Directorypath Filepath::Directory() const
	{
		uint32_t index = m_Path.find_last_of('\\');
		Directorypath directory = Path().substr(0, index);
		return directory;
	}

	blt::string Filepath::Filename() const
	{
		uint32_t index = m_Path.find_last_of('\\');
		return m_Path.substr(index + 1, m_Path.length() - index - 1);
	}

	blt::string Filepath::SimpleFilename() const
	{
		uint32_t index = m_Path.find_last_of('\\');
		uint32_t extIndex = m_Path.find_last_of('.');
		return m_Path.substr(index + 1, index - extIndex - 1);
	}
	
	blt::string Filepath::Extension() const
	{
		uint32_t index = m_Path.find_last_of('.');
		return m_Path.substr(index + 1, m_Path.length() - index - 1);
	}

	bool Filepath::IsRelative() const
	{
		return !IsAbsolute();
	}

	bool Filepath::IsAbsolute() const
	{
		return m_Path.size() > 1 && m_Path[1] == ':';
	}

	void Filepath::MakeAbsolute(const Directorypath& root)
	{
		if (IsRelative())
		{
			m_Path = Filepath::Combine(root, *this).Path();
		}
	}

	std::ostream& operator<<(std::ostream& stream, const Filepath& path)
	{
		stream << path.Path();
		return stream;
	}

	Filepath Filepath::Combine(const Directorypath& directory, const Filepath& file)
	{
		BLT_ASSERT(!(directory.IsAbsolute() && file.IsAbsolute()), "Unable to combine 2 absolute paths");
		BLT_ASSERT(file.IsRelative(), "Right path must be relative in order to combine");
		blt::string leftPath = directory.Path();
		blt::string rightPath = file.Path();
		return Filepath(leftPath + rightPath);
	}

	void Filepath::StandardizePath(blt::string& filepath)
	{
		filepath.replace_all('/', '\\');
	}

}