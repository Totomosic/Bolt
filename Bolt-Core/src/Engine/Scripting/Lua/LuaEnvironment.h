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
		bool HasGlobal(const std::string& name) const;

		template<typename Ret, typename... Args>
		void Register(const std::string& name, std::function<Ret(Args...)> func)
		{
			m_Registry->Register(name, std::move(func));
		}

		template<typename FuncType>
		void Register(const std::string& name, FuncType func)
		{
			Register(name, std::function(std::move(func)));
		}

		template<typename Ret, typename... Args>
		Ret Invoke(const std::string& name, Args... args)
		{
			return m_Registry->Invoke<Ret, Args...>(name, std::move(args)...);
		}

		template<typename T>
		T GetGlobal(const std::string& name)
		{
			CheckLuaError(lua_getglobal(m_State, name.c_str()));
			return lua_help::_check_get<T>(m_State, -1);
		}

		template<typename T>
		void SetGlobal(const std::string& name, const T& value)
		{
			lua_help::_push(m_State, value);
			lua_setglobal(m_State, name.c_str());
		}

	};

}