#include "File.h"
#include "Filesystem.h"

namespace Bolt
{

	File::File(const Filepath& path)
		: m_Path(path)
	{
	
	}

	File::File() : File("")
	{
	
	}

	const blt::string& File::Filename() const
	{
		return m_Path.Path();
	}

	const Filepath& File::Path() const
	{
		return m_Path;
	}

	bool File::IsOpen() const
	{
		return m_Out.is_open() || m_In.is_open();
	}

	int File::FileSize() const
	{
		return Filesystem::FileSize(m_Path);
	}

	void File::Open(OpenFlags flags)
	{
		if (flags == OpenFlags::None)
		{
			m_Out.open(Filename().c_str());
			m_In.open(Filename().c_str());
		}
		else
		{
			m_Out.open(Filename().c_str(), (std::ios::openmode)FlagsToValue(flags));
			m_In.open(Filename().c_str(), (std::ios::openmode)FlagsToValue(flags));
		}
	}

	void File::Close()
	{
		m_Out.close();
		m_In.close();
	}

	void File::Rename(const Filepath& newFilename)
	{
		Filesystem::Rename(m_Path, newFilename);
	}

	void File::Clear()
	{
		Open(OpenFlags::Override);
	}

	void File::Read(char* buffer, int size)
	{
		if (IsOpen())
		{
			m_In.read(buffer, size);
		}
		else
		{
			BLT_ERROR("File was not opened before reading from it. File: " + Filename());
		}
	}

	void File::Read(char* buffer)
	{
		Read(buffer, FileSize());
	}

	char* File::Read(int size)
	{
		char* buffer = new char[size];
		Read(buffer, size);
		return buffer;
	}

	char* File::Read()
	{
		return Read(FileSize());
	}

	void File::ReadText(blt::string* outString)
	{
		std::string str((std::istreambuf_iterator<char>(m_In)), std::istreambuf_iterator<char>());
		*outString = blt::string(str);
	}

	blt::string File::ReadText()
	{
		blt::string str;
		ReadText(&str);
		return str;
	}

	void File::Write(const char* buffer, int size)
	{
		m_Out.write(buffer, size);
	}

	void File::WriteText(const blt::string& text)
	{
		Write(text.c_str(), text.size() * sizeof(char));
	}

	int File::FlagsToValue(OpenFlags flag)
	{
		switch (flag)
		{
		case OpenFlags::None:
			return 0;
		case OpenFlags::Override:
			return (int)std::ios::trunc;
		case OpenFlags::Append:
			return (int)std::ios::app;
		case OpenFlags::Read:
			return (int)std::ios::app;
		}
		return 0;
	}

}