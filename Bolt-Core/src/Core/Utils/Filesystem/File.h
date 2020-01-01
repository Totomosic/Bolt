#pragma once
#include "bltpch.h"
#include "FilePath.h"

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

		const blt::string& Filename() const;
		const FilePath& Path() const;
		bool IsOpen() const;
		uint32_t GetSize() const;
		bool IsReadable() const;
		bool IsWritable() const;

		void Read(void* data, uint32_t size) const;
		void Write(const void* data, uint32_t size) const;

		void ReadText(blt::string* outString, uint32_t size = (uint32_t)-1) const;
		blt::string ReadText(uint32_t size = (uint32_t)-1) const;
		void WriteText(const blt::string& string) const;

		friend class Filesystem;

	private:
		int FlagsToValue(OpenMode mode);
		void SetOpenMode(OpenMode mode);

	};

}