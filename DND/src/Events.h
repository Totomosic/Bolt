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
	constexpr int SERVER_RECEIVED_CAST_SPELL_PACKET_EVENT = USER_EVENT_ID + 6;
	constexpr int SERVER_RECEIVED_STAT_PACKET_EVENT = USER_EVENT_ID + 7;
	constexpr int SERVER_RECEIVED_DEATH_PACKET_EVENT = USER_EVENT_ID + 8;

	constexpr int SERVER_RECIEVED_GET_HOSTS_RESPONSE_PACKET = USER_EVENT_ID + 9;
	constexpr int SERVER_RECIEVED_CLIENT_CONNECTING_PACKET = USER_EVENT_ID + 10;

	constexpr int SERVER_SHUTDOWN_EVENT = USER_EVENT_ID + 50;

	constexpr int STATS_CHANGED_EVENT = USER_EVENT_ID + 100;

	constexpr int SPELL_CAST_EVENT = USER_EVENT_ID + 150;

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