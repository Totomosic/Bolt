#pragma once
#include "bltpch.h"
#include "../Entities/TileMotion.h"

namespace DND
{

	struct PlayerMovePacket;

	class NetworkController : public Component
	{
	public:
		NetworkController();

		std::unique_ptr<Component> Clone() const override;

	};

}