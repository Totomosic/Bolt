#include "Types.h"
#include "Types.h"
#include "PyScript.h"

namespace Bolt
{

	PyScript::PyScript(const char* source)
		: m_Source(source)
	{
	
	}

	const char* PyScript::Source() const
	{
		return m_Source;
	}

	PyScript PyScript::FromFile(const Filepath& file)
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		blt::string source = f.ReadText();
		Filesystem::Close(f);
		return FromSource(source.c_str());
	}

	PyScript PyScript::FromSource(const char* source)
	{
		return PyScript(source);
	}

}