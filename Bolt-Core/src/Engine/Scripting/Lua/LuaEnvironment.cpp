#include "Types.h"
#include "LuaEnvironment.h"

namespace Bolt
{

	LuaEnvironment::LuaEnvironment() : LuaEnvironment(luaL_newstate())
	{
	
	}

	LuaEnvironment::LuaEnvironment(lua_State* state) : LuaState(state)
	{
	
	}

	LuaEnvironment::LuaEnvironment(LuaEnvironment&& other) : LuaState(other.m_State)
	{
		other.m_State = nullptr;
	}

	LuaEnvironment& LuaEnvironment::operator=(LuaEnvironment&& other)
	{
		lua_State* myState = m_State;
		m_State = other.m_State;
		other.m_State = myState;
		return *this;
	}

	LuaEnvironment::~LuaEnvironment()
	{
		if (GetNativeState() != nullptr)
		{
			lua_close(GetNativeState());
		}
	}

	void LuaEnvironment::RegisterFunc(const blt::string& name, LuaEnvironment::LuaRegisteredFunc func)
	{
		lua_register(GetNativeState(), name.c_str(), [](lua_State* state)
			{
				return 0;
			});
	}

}