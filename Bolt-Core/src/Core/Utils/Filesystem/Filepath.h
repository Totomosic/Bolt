#pragma once
#include "Directorypath.h"

namespace Bolt
{

	struct BLT_API Filepath
	{
	private:
		blt::string m_Path;

	public:
		Filepath();
		Filepath(const blt::string& path);
		Filepath(const char* path);

		const blt::string& Path() const;
		Directorypath Directory() const;
		blt::string Filename() const;
		blt::string SimpleFilename() const;
		blt::string Extension() const;
		bool IsRelative() const;
		bool IsAbsolute() const;

		void MakeAbsolute(const Directorypath& root);
		
		friend std::ostream& operator<<(std::ostream& stream, const Filepath& path);

	public:
		static Filepath Combine(const Directorypath& directory, const Filepath& file);

	private:
		static void StandardizePath(blt::string& filepath);

	};

}