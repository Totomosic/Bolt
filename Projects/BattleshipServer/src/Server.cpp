#include "Bolt.h"

using namespace Bolt;

class Server : public Application
{
public:
	std::unique_ptr<TCPserver> m_Server;

public:
	void Init() override
	{
		m_Server = std::make_unique<TCPserver>();
		m_Server->Listen(SocketAddress("localhost", 8000), 5);
		m_Server->Bus().On<TCPconnected>("connected", [](Event<TCPconnected>& e)
			{
				BLT_TRACE("Client connected {}", e.Data.Address);
				e.Data.Client.Bus().On<TCPmessage>("echo", [](Event<TCPmessage>& e)
					{
						BLT_INFO(e.Data.Data.GetBufferPtr());
						e.Data.Socket.Emit("echo", e.Data.Data.GetBufferPtr(), e.Data.Data.GetRemainingDataSize());
					});
			});
	}
};

int main()
{
	EngineCreateInfo info;
	info.UseGraphics = false;
	Engine e(info);
	e.SetApplication<Server>();
	e.Run();
	return 0;
}