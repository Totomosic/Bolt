#include "Types.h"
#include "LuaEnvironment.h"

namespace Bolt
{

	LuaEnvironment::LuaEnvironment() : LuaEnvironment(luaL_newstate())
	{
	
	}

	LuaEnvironment::LuaEnvironment(lua_State* state) : LuaState(state),
		m_Registry(std::make_unique<LuaFunRegistry>(state))
	{
	
	}

	LuaEnvironment::~LuaEnvironment()
	{
		m_Registry.reset(nullptr);
		if (GetNativeState() != nullptr)
		{
			lua_close(GetNativeState());
		}
	}

}