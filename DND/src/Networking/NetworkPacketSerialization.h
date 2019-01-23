#pragma once
#include "NetworkPackets.h"

namespace DND
{

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PacketType& value)
	{
		stream.Write((byte)value);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PacketType& value)
	{
		stream.Read((byte*)&value);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const LocalServerTerminatePacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, LocalServerTerminatePacket& value)
	{
		
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const RegisterAsHostPacket& value)
	{
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, RegisterAsHostPacket& value)
	{
		Deserialize(stream, value.PrivateEndpoint);
	}

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

	template<>
	inline void Serialize(OutputMemoryStream& stream, const GetAllHostsPacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, GetAllHostsPacket& value)
	{

	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const GetAllHostsResponsePacket& value)
	{
		Serialize(stream, value.HostAddresses);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, GetAllHostsResponsePacket& value)
	{
		Deserialize(stream, value.HostAddresses);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const ConnectToAddressPacket& value)
	{
		Serialize(stream, value.Address);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, ConnectToAddressPacket& value)
	{
		Deserialize(stream, value.Address);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DisconnectHostPacket& value)
	{
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DisconnectHostPacket& value)
	{
		Deserialize(stream, value.PrivateEndpoint);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const HolepunchPacket& value)
	{

	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, HolepunchPacket& value)
	{

	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const HolepunchAckPacket& value)
	{
		Serialize(stream, value.MyAddress);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, HolepunchAckPacket& value)
	{
		Deserialize(stream, value.MyAddress);
	}

}