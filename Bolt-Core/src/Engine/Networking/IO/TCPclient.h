#pragma once
#include "../TCPsocket.h"
#include "../InputMemoryStream.h"
#include "../OutputMemoryStream.h"
#include "Core/Events/EventBus.h"

namespace Bolt
{

	class TCPclient;

	struct BLT_API TCPmessage
	{
	public:
		TCPclient& Socket;
		InputMemoryStream Data;
	};

	class BLT_API TCPclient
	{
	private:
		TCPsocket m_Socket;
		GenericEventBus<std::string> m_Events;

	public:
		TCPclient();
		TCPclient(TCPsocket&& socket);

		TCPsocket& GetSocket();
		const GenericEventBus<std::string>& Bus() const;
		GenericEventBus<std::string>& Bus();

		void Connect(const SocketAddress& address);
		void Emit(const std::string& type, const void* data, uint32_t length);
		void Emit(const std::string& type, const std::string& data);
		void Emit(const std::string& type);

	};

}