#pragma once
#include "Command.h"

namespace Bolt
{

	BLT_API enum class CommandProcessError
	{
		CommandNotFound,
		InvalidArgumentType,
		InvalidArgumentCount,
		InvalidCommandToken
	};

	struct BLT_API CommandProcessResult
	{
	public:
		bool Success;
		CommandProcessError Error;
		blt::string ErrDesc;
	};

	class BLT_API CommandLine
	{
	private:
		blt::string m_CommandToken;
		std::unordered_map<blt::string, Command> m_Commands;

	public:
		CommandLine(const blt::string& commandToken = "");

		const blt::string& CommandToken() const;
		const std::unordered_map<blt::string, Command>& Commands() const;
		bool CommandExists(const blt::string& cmdname) const;
		const Command& GetCommand(const blt::string& cmdname) const;

		void AddCommand(const Command& command);
		CommandProcessResult ProcessCommand(const blt::string& command) const;

	};

}