#include "App.h"

int main()
{
	Engine e;
	WindowCreateInfo info;
	info.Title = "Blockstream";
	e.SetWindowCreateInfo(info);
	e.SetApplication<Blockstream::BlockstreamClient>();
	e.Run();
	return 0;
}