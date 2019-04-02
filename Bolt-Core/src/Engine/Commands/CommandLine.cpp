#include "Types.h"
#include "CommandLine.h"

namespace Bolt
{

	CommandLine::CommandLine(const blt::string& cmdToken)
		: m_CommandToken(cmdToken), m_Commands()
	{

	}

	const blt::string& CommandLine::CommandToken() const
	{
		return m_CommandToken;
	}

	const std::unordered_map<blt::string, Command>& CommandLine::Commands() const
	{
		return m_Commands;
	}

	bool CommandLine::CommandExists(const blt::string& cmdname) const
	{
		return m_Commands.find(cmdname) != m_Commands.end();
	}

	const Command& CommandLine::GetCommand(const blt::string& cmdname) const
	{
		return m_Commands.at(cmdname);
	}

	void CommandLine::AddCommand(const Command& command)
	{
		BLT_ASSERT(m_Commands.find(command.Name()) == m_Commands.end(), "Command already exists with name " + command.Name());
		m_Commands[command.Name()] = command;
	}

	CommandProcessResult CommandLine::ProcessCommand(const blt::string& cmd) const
	{
		CommandProcessResult result;
		result.Success = true;
		blt::string command = cmd;
		if (command.find(CommandToken()) != 0)
		{
			result.Success = false;
			result.Error = CommandProcessError::InvalidCommandToken;
			result.ErrDesc = "Command string does not begin with command token: " + CommandToken();
			return result;
		}
		while (command.back() == ' ')
		{
			command.pop_back();
		}
		if (command.contains(' '))
		{
			// Command contains arguments
			std::vector<blt::string> cmdparts = command.split(' ');
			blt::string cmdname = cmdparts.at(0);
			if (!m_CommandToken.empty())
			{
				cmdname.erase(cmdname.begin(), cmdname.begin() + m_CommandToken.size());
			}
			if (m_Commands.find(cmdname) == m_Commands.end())
			{
				result.Success = false;
				result.Error = CommandProcessError::CommandNotFound;
				result.ErrDesc = "No command with name " + cmdname + " exists";
				return result;
			}
			cmdparts.erase(cmdparts.begin()); // Erase name from parts
			const Command& c = m_Commands.at(cmdname);
			if ((int)cmdparts.size() < c.RequiredArgumentCount())
			{
				result.Success = false;
				result.Error = CommandProcessError::InvalidArgumentCount;
				result.ErrDesc = "Command contained invalid number of arguments. Required argument count: " + std::to_string(c.RequiredArgumentCount()) + ", supplied argument count: " + std::to_string(cmdparts.size());
				return result;
			}
			if ((int)cmdparts.size() > c.TotalArgumentCount())
			{
				result.Success = false;
				result.Error = CommandProcessError::InvalidArgumentCount;
				result.ErrDesc = "Command contained invalid number of arguments. Max argument count: " + std::to_string(c.TotalArgumentCount()) + ", supplied argument count: " + std::to_string(cmdparts.size());
				return result;
			}
			c.Callback()(cmdparts);
			return result;
		}
		// Command is only the name of the command
		if (!m_CommandToken.empty())
		{
			command.erase(command.begin(), command.begin() + m_CommandToken.size());
		}
		if (m_Commands.find(command) == m_Commands.end())
		{
			result.Success = false;
			result.Error = CommandProcessError::CommandNotFound;
			result.ErrDesc = "No command with name " + command + " exists";
			return result;
		}
		const Command& c = m_Commands.at(command);
		if (c.RequiredArgumentCount() > 0)
		{
			result.Success = false;
			result.Error = CommandProcessError::InvalidArgumentCount;
			result.ErrDesc = "Command contained invalid number of arguments. Required argument count: " + std::to_string(c.RequiredArgumentCount()) + ", supplied argument count: 0";
			return result;
		}
		c.Callback()({});
		return result;
	}

}