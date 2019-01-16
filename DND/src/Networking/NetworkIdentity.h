#pragma once
#include "bltpch.h"

namespace DND
{

	class NetworkIdentity : public Component
	{
	public:
		id_t NetworkId;
		id_t OwningPlayerId;

	public:
		NetworkIdentity();
		NetworkIdentity(id_t networkId, id_t owningPlayerId);

		std::unique_ptr<Component> Clone() const override;

	};

}