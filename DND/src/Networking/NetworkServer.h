#pragma once
#include "NetworkPacketSerialization.h"
#include "GameStatePacketSerialization.h"
#include "../Events.h"

namespace DND
{

	class NetworkServer
	{
	public:
		using PacketListener = std::function<bool(ReceivedPacketEvent&)>;
		using OnInitCallback = std::function<void()>;
		using OnExitCallback = std::function<void()>;

		struct ListenerId
		{
		public:
			PacketType Type;
			PacketListener* Listener;
		};

		struct ListenerInfo
		{
		public:
			std::unique_ptr<PacketListener> Listener;
			int ListenCount;
		};

	private:
		EventDispatcher<ReceivedPacketEvent> m_OnPacketReceived;
		std::unordered_map<PacketType, std::vector<ListenerInfo>> m_PacketListeners;

		SocketAddress m_SocketAddress;
		UDPsocket m_Socket;
		bool m_IsRunning;

	public:
		NetworkServer();

		const SocketAddress& BoundAddress() const;
		bool IsRunning() const;

		void Initialize(const SocketAddress& bindAddress, OnInitCallback callback);
		void Exit(OnExitCallback callback);

		ListenerId AddPacketListener(PacketType packetType, PacketListener listener);
		ListenerId AddTemporaryPacketListener(PacketType packetType, PacketListener listener, int count);
		void RemovePacketListener(const ListenerId& id);
		void ClearPacketListeners(PacketType packetType);

		template<typename T>
		void SendPacket(const SocketAddress& toAddress, const T& packet)
		{
			if (IsRunning())
			{
				PacketType type = T::Type;
				OutputMemoryStream packetData;
				Serialize(packetData, PACKET_VALIDATOR);
				Serialize(packetData, (byte)type);
				Serialize(packetData, packet);
				int bytesSent = m_Socket.SendTo(toAddress, packetData.GetBufferPtr(), packetData.GetRemainingDataSize());
				BLT_ASSERT(bytesSent == packetData.GetRemainingDataSize(), "UNABLE TO SEND ALL {} BYTES", packetData.GetRemainingDataSize());
			}
		}

	private:
		void ClearAllListeners();
		void StopListeningThread();

	};

}