#include "bltpch.h"
#include "NetworkController.h"
#include "../Networking/Packets.h"
#include "../Entities/TileMotion.h"

namespace DND
{

	NetworkController::NetworkController() : Component()
	{
		
	}

	void NetworkController::OnPlayerMovePacket(const PlayerMovePacket& packet)
	{
		gameObject()->Components().GetComponent<TileMotion>().SetTargetTile(packet.MoveToTile);
	}

	std::unique_ptr<Component> NetworkController::Clone() const
	{
		return std::make_unique<NetworkController>();
	}

}