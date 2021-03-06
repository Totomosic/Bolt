#include "File.h"
#include "Filesystem.h"

namespace Bolt
{

	File::File(const FilePath& path)
		: m_Path(path), m_Stream(), m_Mode(OpenMode::None)
	{
	
	}

	File::File() : File("")
	{
	
	}

	const std::string& File::Filename() const
	{
		return m_Path.Path();
	}

	const FilePath& File::Path() const
	{
		return m_Path;
	}

	bool File::IsOpen() const
	{
		return m_Stream.is_open();
	}

	bool File::IsReadable() const
	{
		return IsOpen() && (m_Mode == OpenMode::Read);
	}

	bool File::IsWritable() const
	{
		return IsOpen() && (m_Mode == OpenMode::Append || m_Mode == OpenMode::Write);
	}

	size_t File::GetSize() const
	{
		BLT_ASSERT(IsOpen(), "File must be open to read size");
		std::streampos begin = m_Stream.tellg();
		m_Stream.seekg(0, std::ios::end);
		std::streampos end = m_Stream.tellg();
		std::streampos size = end - begin;
		m_Stream.seekg(begin);
		return size;
	}

	void File::Read(void* data, size_t size) const
	{
		BLT_ASSERT(IsReadable(), "Unable to read file when not opened for reading");
		m_Stream.read((char*)data, std::min(size, GetSize()));
	}

	void File::Write(const void* data, size_t size) const
	{
		BLT_ASSERT(IsWritable(), "Unable to write to file when not opened for writing");
		m_Stream.write((const char*)data, size);
	}

	void File::ReadText(std::string* outString, size_t size) const
	{
		uint32_t realSize = std::min(size, GetSize());
		char* buffer = new char[realSize];
		Read(buffer, size);
		*outString = std::string(buffer, realSize);
		delete[] buffer;
	}

	std::string File::ReadText(size_t size) const
	{
		std::string result;
		ReadText(&result, size);
		return result;
	}

	void File::WriteText(const std::string& string) const
	{
		Write(string.data(), string.size());
	}

	int File::FlagsToValue(OpenMode mode)
	{
		switch (mode)
		{
		case OpenMode::None:
			return 0;
		case OpenMode::Write:
			return std::ios::trunc | std::ios::binary | std::ios::out | std::ios::in;
		case OpenMode::Append:
			return std::ios::app | std::ios::binary | std::ios::out | std::ios::in;
		case OpenMode::Read:
			return std::ios::app | std::ios::binary | std::ios::in;
		}
		return 0;
	}

	void File::SetOpenMode(OpenMode mode)
	{
		m_Mode = mode;
	}

}