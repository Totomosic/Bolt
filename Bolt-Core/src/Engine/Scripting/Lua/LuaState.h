#pragma once
#include "Bolt-Core.h"
#include "lua.hpp"

namespace Bolt
{

	class BLT_API LuaState
	{
	protected:
		lua_State* m_State;

	public:
		LuaState();
		LuaState(lua_State* state);
		virtual ~LuaState();

		lua_State* GetNativeState() const;

		bool ExecuteString(const blt::string& command) const;
		bool ExecuteFile(const Filepath& file) const;

	protected:
		bool CheckLuaError(int result) const;

	};

}