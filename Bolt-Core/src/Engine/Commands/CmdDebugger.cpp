#include "Types.h"
#include "CmdDebugger.h"

namespace Bolt
{

	std::shared_ptr<spdlog::logger> CmdDebugger::s_Logger;
	CommandLine CmdDebugger::s_CommandLine = CommandLine("/");

	void CmdDebugger::Initialize()
	{
		s_Logger = spdlog::stdout_color_mt("DEBUGGER");
		s_Logger->set_level(spdlog::level::level_enum::trace);

		CreateCommands();

		EventManager::Subscribe(Events::COMMAND_LINE_MESSAGE_RECEIVED, [](id_t eventId, Event& args) -> bool
		{
			CommandLineEvent& e = *(CommandLineEvent*)&args;
			CommandProcessResult result = s_CommandLine.ProcessCommand(e.String);
			if (!result.Success)
			{
				s_Logger->error(result.ErrDesc);
			}
			return false;
		});

		std::thread listenerThread([]()
		{
			char buffer[256];
			while (true)
			{
				std::cin.getline(buffer, 256 * sizeof(char));
				std::unique_ptr<CommandLineEvent> e = std::make_unique<CommandLineEvent>();
				e->String = blt::string(buffer);
				EventManager::Post(Events::COMMAND_LINE_MESSAGE_RECEIVED, std::move(e));
			}
		});
		listenerThread.detach();
	}

	const CommandLine& CmdDebugger::CmdLine()
	{
		return s_CommandLine;
	}

	void CmdDebugger::CreateCommands()
	{
		Command helpCommand("help", [](const Command::CommandArgList& args)
		{
			if (args.size() > 0)
			{
				blt::string cmdname = args[0];
				if (!s_CommandLine.CommandExists(cmdname))
				{
					s_Logger->error("Could not find help for unknown command " + cmdname);
					return;
				}
				const Command& c = s_CommandLine.GetCommand(cmdname);
				s_Logger->info(s_CommandLine.CommandToken() + c.GetHelpString());
			}
			else
			{
				s_Logger->info("Listing commands...");
				for (auto& pair : s_CommandLine.Commands())
				{
					s_Logger->info(s_CommandLine.CommandToken() + pair.second.GetHelpString());
				}
			}
		});
		helpCommand.AddArgument("cmd", CommandArgType::String, true);
		s_CommandLine.AddCommand(helpCommand);
	}

}