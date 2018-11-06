#pragma once
#include "Types.h"
#include "Filepath.h"

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
		Filepath m_Path;
		mutable std::fstream m_Stream;
		mutable OpenMode m_Mode;

	public:
		File();
		File(const Filepath& filepath);

		const blt::string& Filename() const;
		const Filepath& Path() const;
		bool IsOpen() const;
		uint GetSize() const;
		bool IsReadable() const;
		bool IsWritable() const;

		void Read(void* data, uint size) const;
		void Write(const void* data, uint size) const;

		void ReadText(blt::string* outString, uint size = (uint)-1) const;
		blt::string ReadText(uint size = (uint)-1) const;
		void WriteText(const blt::string& string) const;

		friend class Filesystem;

	private:
		int FlagsToValue(OpenMode mode);
		void SetOpenMode(OpenMode mode);

	};

}