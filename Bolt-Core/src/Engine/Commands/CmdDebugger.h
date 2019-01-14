#pragma once
#include "CommandLine.h"

namespace Bolt
{

	class BLT_API CommandLineEvent : public Event
	{
	public:
		blt::string String;
	};

	class BLT_API CmdDebugger
	{
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
		static CommandLine s_CommandLine;

	public:
		static void Initialize();

		static const CommandLine& CmdLine();

	private:
		static void CreateCommands();

	};

}