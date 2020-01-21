#pragma once
#include "Networking.h"
#include "SocketAddress.h"

namespace Bolt
{

	class BLT_API UDPsocket
	{
	private:
		SocketHandle m_Socket;

	public:
		UDPsocket(SocketHandle socket);
		UDPsocket(AddressFamily addressFamily = AddressFamily::INET);
		UDPsocket(const UDPsocket& other) = delete;
		UDPsocket& operator=(const UDPsocket& other) = delete;
		UDPsocket(UDPsocket&& other);
		UDPsocket& operator=(UDPsocket&& other);
		~UDPsocket();

		bool IsValid() const;

		int Connect(const SocketAddress& address);
		int Bind(const SocketAddress& address);
		int SendTo(const SocketAddress& address, const void* data, uint32_t length);
		int RecvFrom(void* buffer, uint32_t length, SocketAddress* outAddress);
		int Shutdown();
		int Close();

		void SetBlocking(bool isBlocking);

		friend class SocketUtil;

	};

}