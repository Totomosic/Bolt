#include "bltpch.h"
#include "LuaFun.h"

namespace Bolt
{

	int LuaDispatcher(lua_State* state)
	{
		LuaFunBase* func = (LuaFunBase*)lua_touserdata(state, lua_upvalueindex(1));
		return func->Execute(state);
	}

}