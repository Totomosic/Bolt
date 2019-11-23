#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct Filepath;

	struct BLT_API Directorypath
	{
	public:
		static constexpr char DIRECTORY_DELIMITER = '/';

	private:
		blt::string m_Path;

	public:
		Directorypath();
		Directorypath(const blt::string& path);
		Directorypath(const char* path);

		const blt::string& Path() const;
		bool HasParent() const;
		Directorypath Parent() const;
		bool IsRelative() const;
		bool IsAbsolute() const;

		void MakeAbsolute(const Directorypath& root);

		bool operator==(const Directorypath& other) const;
		bool operator!=(const Directorypath& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const Directorypath& path);
		friend struct Filepath;

	public:
		static Directorypath Combine(const Directorypath& left, const Directorypath& right);

	private:
		static void StandardizePath(blt::string& directorypath);

	};

}