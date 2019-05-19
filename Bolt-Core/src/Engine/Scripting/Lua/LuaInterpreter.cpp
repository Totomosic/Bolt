#include "bltpch.h"
#include "LuaInterpreter.h"

namespace Bolt
{

	LuaInterpreter::LuaInterpreter()
		: m_Environment()
	{
		Reset();
	}

	const LuaEnvironment& LuaInterpreter::Environment() const
	{
		return m_Environment;
	}

	LuaEnvironment& LuaInterpreter::Environment()
	{
		return m_Environment;
	}

	bool LuaInterpreter::ExecuteString(const blt::string& line) const
	{
		if (m_Environment.ExecuteString(line))
		{
			return true;
		}
		return false;
	}

	void LuaInterpreter::Reset()
	{
		m_Environment.Reset();
	}

}