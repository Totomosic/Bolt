#include "bltpch.h"
#include "TCPclient.h"

namespace Bolt
{

	TCPclient::TCPclient() : TCPclient(TCPsocket())
	{
	
	}

	TCPclient::TCPclient(TCPsocket&& socket)
		: m_Socket(std::move(socket)), m_Events()
	{
		
	}

	TCPsocket& TCPclient::GetSocket()
	{
		return m_Socket;
	}

	const GenericEventBus<std::string>& TCPclient::Bus() const
	{
		return m_Events;
	}

	GenericEventBus<std::string>& TCPclient::Bus()
	{
		return m_Events;
	}

	void TCPclient::Connect(const SocketAddress& address)
	{
		m_Socket.Connect(address);
		std::thread([this]()
			{
				byte buffer[1024];
				while (true)
				{
					int received = m_Socket.Recv(buffer, 1024);
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

						Bus().Emit(typeString, TCPmessage{ *this, std::move(dataStream) });

						BLT_DELETE_ARR typeString;
					}
				}
			}).detach();
	}

	void TCPclient::Emit(const std::string& type, const void* data, uint32_t length)
	{
		OutputMemoryStream stream;
		stream.Write((uint32_t)type.length() + 1);
		stream.Write(type.data(), type.length() + 1);
		stream.Write(length);
		stream.Write(data, length);
		m_Socket.Send(stream.GetBufferPtr(), stream.GetRemainingDataSize());
	}

	void TCPclient::Emit(const std::string& type, const std::string& data)
	{
		Emit(type, data.data(), data.length() + 1);
	}

	void TCPclient::Emit(const std::string& type)
	{
		Emit(type, "");
	}

}