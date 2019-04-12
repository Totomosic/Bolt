#pragma once
#include "LuaEnvironment.h"

namespace Bolt
{

	class BLT_API LuaInterpreter
	{
	private:
		LuaEnvironment m_Environment;

	public:
		LuaInterpreter();

		const LuaEnvironment& Environment() const;
		LuaEnvironment& Environment();

		bool ExecuteString(const blt::string& line) const;

		void Reset();

	};

}