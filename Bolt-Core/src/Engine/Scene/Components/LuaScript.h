#pragma once
#include "Component.h"
#include "Engine/Scripting/Lua/LuaScriptingEnv.h"

namespace Bolt
{

	class BLT_API LuaScript : public Component
	{
	private:
		LuaScriptingEnv m_Environment;
		bool m_HasStart;
		bool m_HasEnd;
		bool m_HasUpdate;
		bool m_HasLateUpdate;

	public:
		LuaScript(const blt::string& scriptText);

		const LuaScriptingEnv& Environment() const;
		LuaScriptingEnv& Environment();

		void Start() override;
		void End() override;
		void Update() override;
		void LateUpdate() override;

		std::unique_ptr<Component> Clone() const override;

	};

}