#pragma once
#include "Debug.h"

namespace Bolt
{

	enum class TextColor : WORD
	{
		Blue = 9,
		Green = 10,
		Teal = 11,
		Red = 12,
		Purple = 13,
		Yellow = 14,
		White = 15
	};

	class Console
	{
	private:
		static HANDLE s_ConsoleHandle;

	public:
		static void SetTextColor(TextColor color);
		static void Write(const std::string& str);
		static void WriteLine(const std::string& str);
		static void WriteError(const std::string& err);

		static void Pause();
		static std::string GetInput();
		static std::string GetInput(const std::string& prompt);
	};

}