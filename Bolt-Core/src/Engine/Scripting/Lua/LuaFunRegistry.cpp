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

	int LuaFunRegistry::LuaDispatcher(lua_State* state)
	{
		LuaFunBase* func = (LuaFunBase*)lua_touserdata(state, lua_upvalueindex(1));
		return func->Execute(state);
	}

}