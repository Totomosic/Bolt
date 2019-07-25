#pragma once
#include "LuaEnvironment.h"

namespace Bolt
{

	void RegisterBaseMethods(LuaEnvironment& env);
	void RegisterGraphicsMethods(LuaEnvironment& env);
	void RegisterSceneMethods(LuaEnvironment& env);
	void RegisterAllMethods(LuaEnvironment& env);

}