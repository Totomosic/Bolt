#pragma once
#include "bltpch.h"
#include "LuaFun.h"

namespace Bolt
{

	class LuaFunBase;

	class BLT_API LuaFunRegistry
	{
	private:
		lua_State* m_State;
		std::vector<std::unique_ptr<LuaFunBase>> m_Funcs;

	public:
		LuaFunRegistry(lua_State* state);

		lua_State* GetLuaState() const;

		template<typename Ret, typename... Args>
		void Register(const blt::string& name, std::function<Ret(Args...)> func)
		{
			auto ptr = std::make_unique<LuaFun<1, Ret, Args...>>(m_State, name, std::move(func));
			m_Funcs.push_back(std::move(ptr));
		}

		template<typename... Args>
		void Register(const blt::string& name, std::function<void(Args...)> func)
		{
			auto ptr = std::make_unique<LuaFun<0, void, Args...>>(m_State, name, std::move(func));
			m_Funcs.push_back(std::move(ptr));
		}

		template<typename Ret, typename... Args>
		Ret Invoke(const blt::string& name, Args... args)
		{
			lua_getglobal(m_State, name.c_str());
			if (!lua_isfunction(m_State, -1))
			{
				BLT_ASSERT(false, "Unable to call function {} as it is not a function", name);
				return Ret();
			}
			// C++17 fold expression https://en.cppreference.com/w/cpp/language/fold
			(lua_help::_push<Args>(m_State, std::forward<Args>(args)), ...);
			lua_call(m_State, sizeof...(Args), 1);
			return lua_help::_check_get<Ret>(m_State, -1);
		}

	public:
		static int LuaDispatcher(lua_State* state);

	};

}