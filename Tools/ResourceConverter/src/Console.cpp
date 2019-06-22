#include "Console.h"

namespace Bolt
{

	HANDLE Console::s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	void Console::SetTextColor(TextColor color)
	{
		SetConsoleTextAttribute(s_ConsoleHandle, (WORD)color);
	}

	void Console::Write(const std::string& str)
	{
		SetTextColor(TextColor::White);
		std::cout << str;
	}

	void Console::WriteLine(const std::string& str)
	{
		SetTextColor(TextColor::White);
		std::cout << str << std::endl;
	}

	void Console::WriteError(const std::string& err)
	{
		SetTextColor(TextColor::Red);
		std::cout << err << std::endl;
		Pause();
	}

	void Console::Pause()
	{
		WriteLine("Press enter to continue...");
		std::cin.get();
	}

	std::string Console::GetInput()
	{
		std::string result;
		std::cin >> result;
		return result;
	}

	std::string Console::GetInput(const std::string& prompt)
	{
		Write(prompt);
		return GetInput();
	}

}