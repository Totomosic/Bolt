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
		GenericEventBus<blt::string> m_Events;

	public:
		TCPclient();
		TCPclient(TCPsocket&& socket);

		TCPsocket& GetSocket();
		const GenericEventBus<blt::string>& Bus() const;
		GenericEventBus<blt::string>& Bus();

		void Connect(const SocketAddress& address);
		void Emit(const blt::string& type, const void* data, uint32_t length);
		void Emit(const blt::string& type, const blt::string& data);
		void Emit(const blt::string& type);

	};

}