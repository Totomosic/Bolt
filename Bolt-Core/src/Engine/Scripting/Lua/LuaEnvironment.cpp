#include "bltpch.h"
#include "LuaEnvironment.h"

#include "LuaBoltMethods.h"

namespace Bolt
{

	LuaEnvironment::LuaEnvironment() : LuaEnvironment(luaL_newstate())
	{
	
	}

	LuaEnvironment::LuaEnvironment(lua_State* state) : LuaState(state),
		m_Registry(std::make_unique<LuaFunRegistry>(state))
	{
		RegisterAllMethods(*this);
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
		RegisterAllMethods(*this);
	}

	bool LuaEnvironment::HasGlobal(const std::string& name) const
	{
		lua_getglobal(GetNativeState(), name.c_str());
		bool result = !lua_isnil(GetNativeState(), -1);
		lua_pop(GetNativeState(), 1);
		return result;
	}

}