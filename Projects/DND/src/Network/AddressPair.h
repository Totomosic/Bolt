#pragma once
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

}