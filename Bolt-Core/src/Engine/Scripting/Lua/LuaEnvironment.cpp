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
		RegisterDefaultFuncs();
	}

	LuaEnvironment::~LuaEnvironment()
	{
		m_Registry.reset(nullptr);
		if (GetNativeState() != nullptr)
		{
			lua_close(GetNativeState());
		}
	}

	void LuaEnvironment::Reset()
	{
		this->~LuaEnvironment();
		m_State = luaL_newstate();
		m_Registry = std::make_unique<LuaFunRegistry>(m_State);
		RegisterDefaultFuncs();
	}

	void LuaEnvironment::RegisterDefaultFuncs()
	{
		Register("Log", [](blt::string msg)
			{
				BLT_LUA_TRACE(msg);
			});
	}

}