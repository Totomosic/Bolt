#include "bltpch.h"
#include "NetworkIdentity.h"

namespace DND
{

	NetworkIdentity::NetworkIdentity()
	{
	
	}

	NetworkIdentity::NetworkIdentity(id_t networkId, id_t owningPlayerId) : Component(),
		NetworkId(networkId), OwningPlayerId(owningPlayerId)
	{
	
	}

	std::unique_ptr<Component> NetworkIdentity::Clone() const
	{
		return std::make_unique<NetworkIdentity>(NetworkId, OwningPlayerId);
	}

}