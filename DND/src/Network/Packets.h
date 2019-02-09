#pragma once
#include "bltpch.h"
#include "Serialization.h"
#include "Deserialization.h"

namespace DND
{

	using PacketTypeDataType = byte;

	constexpr id_t PACKET_VALIDATOR = 0xFFEEDDCC;
	constexpr id_t PACKET_HEADER_SIZE = sizeof(PacketTypeDataType) + sizeof(PACKET_VALIDATOR);

	enum class PacketType : PacketTypeDataType
	{
		Ignore,
		TerminateLocalServer,

		UploadAddressPair,
		AddressPairResponse,
		RegisterAsHost,
		GetAllHosts,
		GetHostsResponse,
		RemoveAsHost,
		ConnectToHost,
		ConnectionEstablished,

		Hello,
		Welcome,
		Introduction,
		Disconnect
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PacketType& value)
	{
		stream.Write((PacketTypeDataType*)&value, sizeof(PacketTypeDataType));
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PacketType& value)
	{
		stream.Read((PacketTypeDataType*)&value, sizeof(PacketTypeDataType));
	}

	class ReceivedPacket : public Event
	{
	public:
		PacketType Type;
		SocketAddress FromAddress;
		InputMemoryStream Packet;
	};

}