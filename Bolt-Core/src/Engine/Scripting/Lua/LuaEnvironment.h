#pragma once
#include "LuaState.h"
#include "LuaFunRegistry.h"

namespace Bolt
{

	class BLT_API LuaEnvironment : public LuaState
	{
	private:
		std::unique_ptr<LuaFunRegistry> m_Registry;

	public:
		LuaEnvironment();
		// Environment owns the lua_State
		LuaEnvironment(lua_State* state);
		~LuaEnvironment() override;

		void Reset();

		template<typename Ret, typename... Args>
		void Register(const blt::string& name, std::function<Ret(Args...)> func)
		{
			m_Registry->Register(name, std::move(func));
		}

		template<typename FuncType>
		void Register(const blt::string& name, FuncType func)
		{
			Register(name, std::function(std::move(func)));
		}

		template<typename Ret, typename... Args>
		Ret Invoke(const blt::string& name, Args... args)
		{
			return m_Registry->Invoke<Ret, Args...>(name, std::move(args)...);
		}

		template<typename T>
		T GetGlobal(const blt::string& name)
		{
			CheckLuaError(lua_getglobal(m_State, name.c_str()));
			return lua_help::_check_get<T>(m_State, -1);
		}

		template<typename T>
		void SetGlobal(const blt::string& name, const T& value)
		{
			lua_help::_push(m_State, value);
			CheckLuaError(lua_setglobal(m_State, name.c_str()));
		}

	protected:
		void RegisterDefaultFuncs();

	};

}