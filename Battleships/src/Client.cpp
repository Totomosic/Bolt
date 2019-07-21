#include "Bolt.h"

using namespace Bolt;

class Client : public Application
{
public:
	std::unique_ptr<TCPclient> client;

public:
	void Init() override
	{
		client = std::make_unique<TCPclient>();
		client->Connect(SocketAddress("localhost", 8000));
		client->Bus().On<TCPmessage>("echo", [](Event<TCPmessage>& e)
			{
				BLT_INFO(e.Data.Data.GetBufferPtr());
				e.Data.Socket.Emit("echo", e.Data.Data.GetBufferPtr(), e.Data.Data.GetRemainingDataSize());
			});
		std::this_thread::sleep_for(std::chrono::seconds(1));
		client->Emit("echo", std::to_string(Random::NextInt(0, 10)));
	}
};

int main()
{
	EngineCreateInfo info;
	info.UseGraphics = false;
	Engine e(info);
	e.SetApplication<Client>();
	e.Run();
	return 0;
}