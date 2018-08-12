#pragma once
#include "Types.h"
#include "Filepath.h"

namespace Bolt
{

	enum class OpenFlags : int
	{
		None,
		Override,
		Read,
		Append
	};

	class BLT_API File
	{
	private:
		Filepath m_Path;
		std::ofstream m_Out;
		std::ifstream m_In;

	public:
		File();
		File(const Filepath& filepath);

		const blt::string& Filename() const;
		const Filepath& Path() const;
		bool IsOpen() const;
		int FileSize() const;

		void Open(OpenFlags flags = OpenFlags::Read);
		void Close();
		void Rename(const Filepath& newFilename);
		void Clear();

		void Read(char* buffer, int size);
		void Read(char* buffer);
		char* Read(int size);
		char* Read();
		void ReadText(blt::string* outString);
		blt::string ReadText();

		void Write(const char* buffer, int size);
		void WriteText(const blt::string& text);

		friend class Filesystem;

	private:
		int FlagsToValue(OpenFlags flags);

	};

}