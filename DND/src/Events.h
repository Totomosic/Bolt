#pragma once
#include "Networking/Packets.h"

namespace DND
{

	class NetworkServer;

	constexpr int SERVER_RECEIVED_PACKET_EVENT = USER_EVENT_ID + 0;
	constexpr int SERVER_RECEIVED_HELLO_PACKET_EVENT = USER_EVENT_ID + 1;
	constexpr int SERVER_RECEIVED_WELCOME_PACKET_EVENT = USER_EVENT_ID + 2;
	constexpr int SERVER_RECEIVED_INTRODUCTION_PACKET_EVENT = USER_EVENT_ID + 3;
	constexpr int SERVER_RECEIVED_DISCONNECT_PACKET_EVENT = USER_EVENT_ID + 4;

	constexpr int SERVER_RECEIVED_PLAYER_MOVE_PACKET_EVENT = USER_EVENT_ID + 5;

	class ReceivedPacketEvent : public Event
	{
	public:
		SocketAddress FromAddress;
		NetworkServer* Server;
		PacketType Type;
		id_t PacketId;
		InputMemoryStream Packet;
	};

}