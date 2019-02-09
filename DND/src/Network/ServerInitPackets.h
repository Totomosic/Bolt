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

	struct RegisterAsHostPacket
	{
	public:
		static constexpr PacketType Type = PacketType::RegisterAsHost;

	public:
		SocketAddress PrivateEndpoint;
	};

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

	struct RemoveAsHostPacket
	{
	public:
		static constexpr PacketType Type = PacketType::RemoveAsHost;

	public:
		SocketAddress PrivateEndpoint;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const RemoveAsHostPacket& value)
	{
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, RemoveAsHostPacket& value)
	{
		Deserialize(stream, value.PrivateEndpoint);
	}

	struct GetAllHostsPacket
	{
	public:
		static constexpr PacketType Type = PacketType::GetAllHosts;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const GetAllHostsPacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, GetAllHostsPacket& value)
	{
		
	}

	struct GetHostsResponsePacket
	{
	public:
		static constexpr PacketType Type = PacketType::GetHostsResponse;

	public:
		std::vector<AddressPair> Hosts;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const GetHostsResponsePacket& value)
	{
		Serialize(stream, value.Hosts);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, GetHostsResponsePacket& value)
	{
		Deserialize(stream, value.Hosts);
	}

	struct ConnectToHostPacket
	{
	public:
		static constexpr PacketType Type = PacketType::ConnectToHost;
		
	public:
		AddressPair HostAddress;
		SocketAddress PrivateEndpoint;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const ConnectToHostPacket& value)
	{
		Serialize(stream, value.HostAddress);
		Serialize(stream, value.PrivateEndpoint);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, ConnectToHostPacket& value)
	{
		Deserialize(stream, value.HostAddress);
		Deserialize(stream, value.PrivateEndpoint);
	}

	struct ConnectionEstablishedPacket
	{
	public:
		static constexpr PacketType Type = PacketType::ConnectionEstablished;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const ConnectionEstablishedPacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, ConnectionEstablishedPacket& value)
	{
		
	}

}