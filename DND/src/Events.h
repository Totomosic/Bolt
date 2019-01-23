#pragma once
#include "Networking/NetworkPackets.h"

namespace DND
{

	class NetworkServer;

	constexpr int USER_EVENT_ID = EventManager::USER_EVENT_ID;

	constexpr int SERVER_RECEIVED_PACKET_EVENT = USER_EVENT_ID + 0;
	constexpr int SERVER_SHUTDOWN_EVENT = USER_EVENT_ID + 1;

	constexpr int STATS_CHANGED_EVENT = USER_EVENT_ID + 50;

	class ReceivedPacketEvent : public Event
	{
	public:
		SocketAddress FromAddress;
		NetworkServer* Server;
		PacketType Type;
		InputMemoryStream Packet;
	};

}