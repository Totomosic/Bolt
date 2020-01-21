#include "bltpch.h"
#include "TCPserver.h"

namespace Bolt
{

	TCPserver::TCPserver()
		: m_ClientMutex(), m_Socket(), m_Events(), m_Clients()
	{
	
	}

	const GenericEventBus<std::string>& TCPserver::Bus() const
	{
		return m_Events;
	}

	GenericEventBus<std::string>& TCPserver::Bus()
	{
		return m_Events;
	}

	void TCPserver::Listen(const SocketAddress& address, int backlog)
	{
		m_Socket.Bind(address);
		m_Socket.Listen(backlog);
		std::thread([this]()
			{
				while (true)
				{
					SocketAddress clientAddr;
					TCPsocket clientSocket = m_Socket.Accept(&clientAddr);
					{
						std::scoped_lock<std::mutex> lock(m_ClientMutex);
						std::unique_ptr<TCPclient> client = std::make_unique<TCPclient>(std::move(clientSocket));
						TCPclient* clientPtr = client.get();
						m_Clients.push_back(std::move(client));
						m_Events.Emit("connected", TCPconnected{ *clientPtr, std::move(clientAddr) });
						std::thread([clientPtr]()
							{
								byte buffer[1024];
								while (true)
								{
									int received = clientPtr->GetSocket().Recv(buffer, 1024);
									if (received > 0)
									{
										InputMemoryStream stream(received);
										memcpy(stream.GetBufferPtr(), buffer, received);
										uint32_t typeLength;
										uint32_t dataLength;
										stream.Read(&typeLength);
										char* typeString = BLT_NEW char[typeLength];
										stream.Read(typeString, typeLength);
										stream.Read(&dataLength);
										InputMemoryStream dataStream(dataLength);
										stream.Read(dataStream.GetBufferPtr(), dataLength);

										clientPtr->Bus().Emit(typeString, TCPmessage{ *clientPtr, std::move(dataStream) });

										BLT_DELETE_ARR typeString;
									}
								}
							}).detach();
					}
				}
			}).detach();
	}

	void TCPserver::Broadcast(const std::string& type, const void* data, uint32_t length)
	{
	
	}

	void TCPserver::Broadcast(const std::string& type, const std::string& data)
	{
	
	}

	void TCPserver::Broadcast(const std::string& type)
	{
	
	}

}