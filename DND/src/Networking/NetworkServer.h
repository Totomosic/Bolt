#pragma once
#include "Packets.h"
#include "../Events.h"

namespace DND
{

	class PacketValidator
	{
	public:
		id_t m_CurrentPacketId;

	public:
		PacketValidator();

		id_t GetNextPacketId();
		bool ValidateIncomingPacketId(id_t id);

	};

	class NetworkServer
	{
	public:
		EventDispatcher<ReceivedPacketEvent> OnHelloPacket;
		EventDispatcher<ReceivedPacketEvent> OnWelcomePacket;
		EventDispatcher<ReceivedPacketEvent> OnIntroductionPacket;
		EventDispatcher<ReceivedPacketEvent> OnDisconnectPacket;

	private:
		bool m_IsRunning;
		SocketAddress m_Address;
		UDPsocket m_Socket;
		std::unordered_map<SocketAddress, PacketValidator> m_Validators;

	public:
		NetworkServer();

		inline const SocketAddress& Address() const { return m_Address; }

		void SetAddress(const SocketAddress& address);
		void Initialize(bool runListenThread);
		void Terminate();

		void RunListenThread();

		template<typename T>
		void SendPacket(const SocketAddress& to, const T& packet)
		{
			PacketValidator& validator = m_Validators[to];
			id_t packetId = validator.GetNextPacketId();
			PacketType pType = TypeOfPacket<T>();
			OutputMemoryStream pack;
			Serialize(pack, packetId);
			Serialize(pack, (byte)pType);
			Serialize(pack, packet);
			int bytesSent = m_Socket.SendTo(to, pack.GetBufferPtr(), pack.GetRemainingDataSize());
			BLT_ASSERT(bytesSent == pack.GetRemainingDataSize(), "Unable to send full packet with size: " + std::to_string(pack.GetRemainingDataSize()));
		}

	private:
		void StopListeningThread();
		void DispatchPacketEvent(EventDispatcher<ReceivedPacketEvent>& dispatcher, std::unique_ptr<ReceivedPacketEvent>&& args);
		EventDispatcher<ReceivedPacketEvent>& GetEventDispatcher(PacketType type);

	};

}