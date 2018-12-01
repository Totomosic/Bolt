#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API PyScript
	{
	private:
		const char* m_Source;

	private:
		PyScript(const char* source);

	public:
		const char* Source() const;

	public:
		static PyScript FromSource(const char* source);
		static PyScript FromFile(const Filepath& file);

	};

}