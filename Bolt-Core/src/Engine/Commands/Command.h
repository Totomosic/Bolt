#pragma once
#include "CommandArg.h"

namespace Bolt
{

	class BLT_API Command
	{
	public:
		using CommandArgList = std::vector<blt::string>;
		using CommandCallback = std::function<void(const CommandArgList&)>;

	private:
		blt::string m_Name;
		std::vector<CommandArg> m_Arguments;
		CommandCallback m_Callback;

	public:
		Command();
		Command(const blt::string& name, const CommandCallback& callback);

		const blt::string& Name() const;
		const std::vector<CommandArg>& Arguments() const;
		const CommandCallback& Callback() const;
		int TotalArgumentCount() const;
		int RequiredArgumentCount() const;
		int OptionalArgumentCount() const;
		blt::string GetHelpString() const;

		void AddArgument(const blt::string& name, CommandArgType type, bool isOptional = false);

	};

}