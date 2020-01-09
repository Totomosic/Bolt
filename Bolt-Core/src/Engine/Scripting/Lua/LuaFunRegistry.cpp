#include "bltpch.h"
#include "LuaFunRegistry.h"
#include "LuaFun.h"

namespace Bolt
{

	LuaFunRegistry::LuaFunRegistry(lua_State* state)
		: m_State(state), m_Funcs()
	{
	
	}

	lua_State* LuaFunRegistry::GetLuaState() const
	{
		return m_State;
	}

}