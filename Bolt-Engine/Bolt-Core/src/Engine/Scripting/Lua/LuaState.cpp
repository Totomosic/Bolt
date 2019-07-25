#include "bltpch.h"
#include "LuaState.h"

namespace Bolt
{

	LuaState::LuaState() : LuaState(nullptr)
	{
	
	}

	LuaState::LuaState(lua_State* state)
		: m_State(state)
	{
		
	}

	LuaState::~LuaState()
	{
	
	}

	lua_State* LuaState::GetNativeState() const
	{
		return m_State;
	}

	bool LuaState::ExecuteString(const blt::string& command) const
	{
		return CheckLuaError(luaL_dostring(GetNativeState(), command.c_str()));
	}

	bool LuaState::ExecuteFile(const Filepath& file) const
	{
		return CheckLuaError(luaL_dofile(GetNativeState(), file.Path().c_str()));
	}

	bool LuaState::CheckLuaError(int result) const
	{
		if (result != LUA_OK)
		{
			blt::string errorMsg = lua_tostring(GetNativeState(), -1);
			BLT_LUA_ERROR(errorMsg);
			return false;
		}
		return true;
	}

}