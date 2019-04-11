#pragma once
#include "Types.h"
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

	public:
		static int LuaDispatcher(lua_State* state);

	};

}