#pragma once
#include "Packets.h"
#include "Serialization.h"
#include "Deserialization.h"
#include "AddressPair.h"

namespace DND
{

	struct UploadAddressPacket
	{
	public:
		static constexpr PacketType Type = PacketType::UploadAddressPair;

	public:
		SocketAddress PrivateEndpoint;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const UploadAddressPacket& value)
	{
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, UploadAddressPacket& value)
	{
		Deserialize(stream, value.PrivateEndpoint);
	}

	struct AddressPairResponsePacket
	{
	public:
		static constexpr PacketType Type = PacketType::AddressPairResponse;

	public:
		AddressPair Address;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const AddressPairResponsePacket& value)
	{
		Serialize(stream, value.Address);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, AddressPairResponsePacket& value)
	{
		Deserialize(stream, value.Address);
	}

}