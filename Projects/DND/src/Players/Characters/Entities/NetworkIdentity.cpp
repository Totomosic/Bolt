#include "bltpch.h"
#include "NetworkIdentity.h"

namespace DND
{

	NetworkIdentity::NetworkIdentity(id_t networkId) : Component(),
		NetworkId(networkId)
	{
	
	}

	std::unique_ptr<Component> NetworkIdentity::Clone() const
	{
		return std::make_unique<NetworkIdentity>(NetworkId);
	}

}