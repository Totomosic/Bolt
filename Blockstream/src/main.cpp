#include "App.h"

int main()
{
	Engine e;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Blockstream"));
	e.SetApplication(std::make_unique<Blockstream::BlockstreamClient>());
	e.Run();
	return 0;
}