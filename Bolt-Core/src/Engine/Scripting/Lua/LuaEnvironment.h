#pragma once
#include "LuaState.h"

namespace Bolt
{

	class BLT_API LuaEnvironment : public LuaState
	{
	public:
		using LuaRegisteredFunc = std::function<int(LuaEnvironment&)>;
		using InternalLuaFunc = std::function<int(lua_State*)>;

	public:
		LuaEnvironment();
		// Environment owns the lua_State
		LuaEnvironment(lua_State* state);
		LuaEnvironment(const LuaEnvironment& other) = delete;
		LuaEnvironment& operator=(const LuaEnvironment& other) = delete;
		LuaEnvironment(LuaEnvironment&& other);
		LuaEnvironment& operator=(LuaEnvironment&& other);
		~LuaEnvironment() override;


		// Implement design: https://www.jeremyong.com/lua/c++11/templates/metaprogramming/2014/01/15/interfacing-lua-with-templates-in-c-plus-plus-11-continued.html
		void RegisterFunc(const blt::string& name, LuaRegisteredFunc func);

	};

}