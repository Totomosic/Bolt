#pragma once
#include "bltpch.h"
#include "Events.h"
#include "Packets.h"

#include "Serialization.h"
#include "Deserialization.h"

namespace DND
{

	struct TerminateServerPacket
	{
	public:
		static constexpr PacketType Type = PacketType::TerminateLocalServer;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const TerminateServerPacket& value)
	{
	
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, TerminateServerPacket& value)
	{
	
	}

	class ServerShutdownEvent : public Event
	{
	public:
		BLT_EVENT_ID_DEF(SERVER_SHUTDOWN_EVENT);
	};

	class NetworkServer
	{
	public:
		using PacketReceivedCallback = std::function<bool(ReceivedPacket&)>;
		using ListenerTimeoutCallback = std::function<void()>;
		using ServerTerminatedCallback = std::function<void()>;

		struct ListenerInfo
		{
		public:
			PacketReceivedCallback Callback;
			float SecondsRemaining;
			ListenerTimeoutCallback TimeoutCallback;
		};

	private:
		EventDispatcher<ReceivedPacket> m_OnReceivedPacket;

		SocketAddress m_BoundAddress;
		UDPsocket m_Socket;
		std::unordered_map<PacketType, std::vector<ListenerInfo>> m_Callbacks;

	public:
		NetworkServer(SocketAddress address);

		const SocketAddress& Address() const;
		void SetAddress(SocketAddress address);

		void Initialize();
		void Terminate(ServerTerminatedCallback callback);
		void AddPacketListener(PacketType type, PacketReceivedCallback listener);
		void AddPacketListenerTimeout(PacketType type, float timeoutSeconds, PacketReceivedCallback listener, ListenerTimeoutCallback timeoutCallback);
		void ClearPacketListeners();

		void Update(float deltaTime);

		template<typename T>
		void SendPacket(const SocketAddress& address, const T& packet)
		{
			OutputMemoryStream packetStream;
			Serialize(packetStream, PACKET_VALIDATOR);
			Serialize(packetStream, T::Type);
			Serialize(packetStream, packet);
			int bytesSent = m_Socket.SendTo(address, packetStream.GetBufferPtr(), packetStream.GetRemainingDataSize());
		}

	private:
		bool Bind();		
		void TerminateServer();
		
	};

}