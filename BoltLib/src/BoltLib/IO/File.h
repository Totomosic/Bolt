#pragma once
#include "BoltDefines.h"
#include "FilePath.h"
#include <fstream>

namespace Bolt
{

	BLT_API enum class OpenMode : int
	{
		None,
		Write,
		Read,
		Append
	};

	class BLT_API File
	{
	private:
		FilePath m_Path;
		mutable std::fstream m_Stream;
		mutable OpenMode m_Mode;

	public:
		File();
		File(const FilePath& filepath);

		const std::string& Filename() const;
		const FilePath& Path() const;
		bool IsOpen() const;
		size_t GetSize() const;
		bool IsReadable() const;
		bool IsWritable() const;

		void Read(void* data, size_t size) const;
		void Write(const void* data, size_t size) const;

		void ReadText(std::string* outString, size_t size = (size_t)-1) const;
		std::string ReadText(size_t size = (size_t)-1) const;
		void WriteText(const std::string& string) const;

		friend class Filesystem;

	private:
		int FlagsToValue(OpenMode mode);
		void SetOpenMode(OpenMode mode);

	};

}