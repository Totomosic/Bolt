#include "Types.h"
#include "Command.h"

namespace Bolt
{

	Command::Command()
	{
	
	}

	Command::Command(const blt::string& name, const Command::CommandCallback& callback)
		: m_Name(name), m_Arguments(), m_Callback(callback)
	{
	
	}

	const blt::string& Command::Name() const
	{
		return m_Name;
	}

	const std::vector<CommandArg>& Command::Arguments() const
	{
		return m_Arguments;
	}

	const Command::CommandCallback& Command::Callback() const
	{
		return m_Callback;
	}

	int Command::TotalArgumentCount() const
	{
		return m_Arguments.size();
	}

	int Command::RequiredArgumentCount() const
	{
		int count = 0;
		for (const CommandArg& arg : Arguments())
		{
			if (!arg.IsOptional)
			{
				count++;
			}
		}
		return count;
	}

	int Command::OptionalArgumentCount() const
	{
		int count = 0;
		for (const CommandArg& arg : Arguments())
		{
			if (arg.IsOptional)
			{
				count++;
			}
		}
		return count;
	}

	blt::string Command::GetHelpString() const
	{
		blt::string str = Name();
		for (const CommandArg& arg : Arguments())
		{
			str += blt::string(" ") + ((arg.IsOptional) ? "[" : "<") + arg.Name + ((arg.IsOptional) ? "]" : ">");
		}
		return str;
	}

	void Command::AddArgument(const blt::string& name, CommandArgType type, bool isOptional)
	{
		m_Arguments.push_back({ name, type, isOptional });
	}

}