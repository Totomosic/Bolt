#pragma once
#include "LuaUtil.h"

namespace Bolt
{

	class BLT_API LuaFunBase
	{
	public:
		virtual ~LuaFunBase() = default;
		virtual int Execute(lua_State* state) = 0;
	};

	template<size_t N, typename Ret, typename... Args>
	class BLT_API LuaFun : public LuaFunBase
	{
	public:
		using function_t = std::function<Ret(Args...)>;

	private:
		function_t m_Function;
		blt::string m_Name;
		lua_State* m_State;

	public:
		LuaFun(lua_State* l, const blt::string& name, function_t func)
			: m_Function(std::move(func)), m_Name(name), m_State(l)
		{
			lua_pushlightuserdata(m_State, (void*)static_cast<LuaFunBase*>(this));
			lua_pushcclosure(m_State, &LuaFunRegistry::LuaDispatcher, 1);
			lua_setglobal(m_State, m_Name.c_str());
		}

		LuaFun(const LuaFun& other) = delete;
		LuaFun& operator=(const LuaFun& other) = delete;
		LuaFun(LuaFun&& other) = delete;
		LuaFun& operator=(LuaFun&& other) = delete;

		~LuaFun() override
		{
			lua_pushnil(m_State);
			lua_setglobal(m_State, m_Name.c_str());
		}

		int Execute(lua_State* state) override
		{
			return ExecuteAs<Ret>(state);
		}

		template<typename RetType>
		int ExecuteAs(lua_State* state)
		{
			std::tuple<Args...> args = lua_help::_get_args<Args...>(state);
			Ret value = lua_help::_lift(m_Function, std::move(args));
			lua_help::_push(state, std::forward<Ret>(value));
			return (int)N;
		}

		template<>
		int ExecuteAs<void>(lua_State* state)
		{
			std::tuple<Args...> args = lua_help::_get_args<Args...>(state);
			lua_help::_lift(m_Function, std::move(args));
			return (int)N;
		}

	};

}