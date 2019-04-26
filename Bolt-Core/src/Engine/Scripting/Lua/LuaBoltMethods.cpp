#include "Types.h"
#include "LuaBoltMethods.h"

namespace Bolt
{
	
	void RegisterBaseMethods(LuaEnvironment& env)
	{
		env.Register("Bolt_Log", [](blt::string msg)
			{
				BLT_LUA_TRACE(msg);
			});
		env.Register("Bolt_GetRealTime", []()
			{
				return Time::Get().RenderingTimeline().CurrentRealTime();
			});
		env.Register("Bolt_GetTime", []()
			{
				return Time::Get().RenderingTimeline().CurrentTime();
			});
		env.Register("Bolt_GetDeltaTime", []()
			{
				return Time::Get().RenderingTimeline().DeltaTime();
			});
		env.Register("Bolt_ReadTextFile", [](blt::string filename)
			{
				File f = Filesystem::Open(filename, OpenMode::Read);
				blt::string data = f.ReadText();
				Filesystem::Close(f);
				return data;
			});
		env.Register("Bolt_WriteTextFile", [](blt::string filename, blt::string data)
			{
				File f = Filesystem::Open(filename, OpenMode::Write);
				f.WriteText(data);
				Filesystem::Close(f);
			});
		env.Register("Bolt_AppendTextFile", [](blt::string filename, blt::string data)
			{
				File f = Filesystem::Open(filename, OpenMode::Append);
				f.WriteText(data);
				Filesystem::Close(f);
			});
	}

	void RegisterGraphicsMethods(LuaEnvironment& env)
	{
	
	}

	void RegisterSceneMethods(LuaEnvironment& env)
	{
	
	}

	void RegisterAllMethods(LuaEnvironment& env)
	{
		RegisterBaseMethods(env);
		RegisterGraphicsMethods(env);
		RegisterSceneMethods(env);
	}

}