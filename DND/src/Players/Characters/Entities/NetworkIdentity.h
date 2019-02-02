#pragma once
#include "bltpch.h"

namespace DND
{

	class NetworkIdentity : public Component
	{
	public:
		id_t NetworkId;

	public:
		NetworkIdentity(id_t networkId);

		std::unique_ptr<Component> Clone() const override;

	};

}