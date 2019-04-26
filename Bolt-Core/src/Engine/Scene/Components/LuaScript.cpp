#include "Types.h"
#include "LuaScript.h"

namespace Bolt
{

	LuaScript::LuaScript(const blt::string& scriptText) : Component(),
		m_Environment(), m_HasStart(false), m_HasEnd(false), m_HasUpdate(false), m_HasLateUpdate(false)
	{
		m_Environment.ExecuteString(scriptText);
		m_HasStart = m_Environment.HasGlobal("Start");
		m_HasEnd = m_Environment.HasGlobal("End");
		m_HasUpdate = m_Environment.HasGlobal("Update");
		m_HasLateUpdate = m_Environment.HasGlobal("LateUpdate");
	}

	const LuaScriptingEnv& LuaScript::Environment() const
	{
		return m_Environment;
	}

	LuaScriptingEnv& LuaScript::Environment()
	{
		return m_Environment;
	}

	void LuaScript::Start()
	{
		m_Environment.SetGlobal("GameObject", (int)gameObject());
		if (m_HasStart)
		{
			m_Environment.Invoke<void>("Start");
		}
	}

	void LuaScript::End()
	{
		if (m_HasEnd)
		{
			m_Environment.Invoke<void>("End");
		}
	}

	void LuaScript::Update()
	{
		if (m_HasUpdate)
		{
			m_Environment.Invoke<void>("Update");
		}
	}

	void LuaScript::LateUpdate()
	{
		if (m_HasLateUpdate)
		{
			m_Environment.Invoke<void>("LateUpdate");
		}
	}

	std::unique_ptr<Component> LuaScript::Clone() const
	{
		BLT_ASSERT(false, "Unable to clone lua script");
		return nullptr;
	}

}