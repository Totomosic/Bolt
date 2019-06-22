#include "Debug.h"
#include "Console.h"
#include "ResourceConverter.h"

int main()
{
	Bolt::Log::Initialize();
	BLT_CORE_TRACE("Logger Initialized");
	Bolt::Console::WriteLine("Hello world");
	Bolt::Console::WriteError("ERROR");
	return 0;
}