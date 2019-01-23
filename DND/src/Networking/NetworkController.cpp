#include "bltpch.h"
#include "NetworkController.h"
#include "../Networking/NetworkPackets.h"
#include "../Entities/TileMotion.h"

namespace DND
{

	NetworkController::NetworkController() : Component()
	{
		
	}

	void NetworkController::MoveToTile(const Tile& tile) const
	{
		gameObject()->Components().GetComponent<TileMotion>().SetTargetTile(tile);
	}

	std::unique_ptr<Component> NetworkController::Clone() const
	{
		return std::make_unique<NetworkController>();
	}

}