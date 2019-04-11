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

		template<typename Ret, typename... Args>
		void Register(const blt::string& name, std::function<Ret(Args...)> func)
		{
			m_Registry->Register(name, std::move(func));
		}

	};

}