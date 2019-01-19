#pragma once
#include "Networking.h"
#include "SocketAddress.h"

namespace Bolt
{

	class BLT_API UDPsocket
	{
	private:
		SOCKET m_Socket;

	public:
		UDPsocket(SOCKET socket);
		UDPsocket(AddressFamily addressFamily = AddressFamily::INET);
		UDPsocket(const UDPsocket& other) = delete;
		UDPsocket& operator=(const UDPsocket& other) = delete;
		UDPsocket(UDPsocket&& other);
		UDPsocket& operator=(UDPsocket&& other);
		~UDPsocket();

		bool IsValid() const;

		int Bind(const SocketAddress& address);
		int SendTo(const SocketAddress& address, const void* data, uint length);
		int RecvFrom(void* buffer, uint length, SocketAddress* outAddress);
		int Shutdown();
		int Close();

		void SetBlocking(bool isBlocking);

		friend class SocketUtil;

	};

}