#pragma once
#include "bltpch.h"

#include "Serialization.h"
#include "Deserialization.h"

namespace DND
{

	struct AddressPair
	{
	public:
		SocketAddress PublicEndpoint;
		SocketAddress PrivateEndpoint;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const AddressPair& value)
	{
		Serialize(stream, value.PublicEndpoint);
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, AddressPair& value)
	{
		Deserialize(stream, value.PublicEndpoint);
		Deserialize(stream, value.PrivateEndpoint);
	}

	enum class PacketType : byte
	{
		LocalServerTerminate,

		RegisterAsHost,
		AddressPairResponse,
		GetAllHosts,
		GetAllHostsResponse,
		ConnectToAddress,
		DisconnectHost,

		Holepunch,
		HolepunchAck,
		ConnectionEstablished,

		Hello,
		Welcome,
		Introduction,
		NotAcceptingClients,
		PlayerDisconnect,

		EntityMoved,
		CastSpell,
		StatsUpdate,

		CreateNPC,
		DestroyNPC,

		MAX_PACKET_TYPES
	};

	constexpr id_t PACKET_VALIDATOR = 0xFFEEDDCC;
	constexpr id_t PACKET_HEADER_SIZE = sizeof(id_t) + sizeof(PacketType);

	struct LocalServerTerminatePacket
	{
	public:
		constexpr static PacketType Type = PacketType::LocalServerTerminate;
	};

	struct RegisterAsHostPacket
	{
	public:
		constexpr static PacketType Type = PacketType::RegisterAsHost;
		
	public:
		SocketAddress PrivateEndpoint;
	};

	struct AddressPairResponsePacket
	{
	public:
		constexpr static PacketType Type = PacketType::AddressPairResponse;

	public:
		AddressPair Address;
	};

	struct GetAllHostsPacket
	{
	public:
		constexpr static PacketType Type = PacketType::GetAllHosts;
	};

	struct GetAllHostsResponsePacket
	{
	public:
		constexpr static PacketType Type = PacketType::GetAllHostsResponse;
		
	public:
		std::vector<AddressPair> HostAddresses;
	};

	struct ConnectToAddressPacket
	{
	public:
		constexpr static PacketType Type = PacketType::ConnectToAddress;
		
	public:
		AddressPair Address;
	};

	struct DisconnectHostPacket
	{
	public:
		constexpr static PacketType Type = PacketType::DisconnectHost;

	public:
		SocketAddress PrivateEndpoint;
	};

	struct HolepunchPacket
	{
	public:
		constexpr static PacketType Type = PacketType::Holepunch;

	public:
		AddressPair MyAddress;
	};

	struct HolepunchAckPacket
	{
	public:
		constexpr static PacketType Type = PacketType::HolepunchAck;

	public:
		AddressPair MyAddress;
	};

	struct ConnectionEstablishedPacket
	{
	public:
		constexpr static PacketType Type = PacketType::ConnectionEstablished;
	};

}