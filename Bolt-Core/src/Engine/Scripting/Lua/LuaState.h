#pragma once
#include "Core/Utils/Filesystem/FilePath.h"

namespace Bolt
{

	class BLT_API LuaState
	{
	protected:
		lua_State* m_State;

	public:
		LuaState();
		LuaState(lua_State* state);
		virtual ~LuaState();

		lua_State* GetNativeState() const;

		bool ExecuteString(const blt::string& command) const;
		bool ExecuteFile(const FilePath& file) const;

	protected:
		bool CheckLuaError(int result) const;

	};

}