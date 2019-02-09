#include "bltpch.h"
#include "MotionController.h"

#include "../../../GameplayPackets.h"

namespace DND
{

	MotionController::MotionController()
		: m_TileMotion(nullptr), m_Network(nullptr)
	{

	}

	void MotionController::Initialize(GameObject* object)
	{
		m_TileMotion = &object->Components().GetComponent<TileMotion>();
		m_Network = &object->Components().GetComponent<NetworkComponent>();
	}

	void MotionController::MoveToBottomLeftTile(const Tile& targetTile) const
	{
		m_TileMotion->MoveToBottomLeftTile(targetTile);
		MoveToTilePacket packet;
		packet.NetworkId = m_Network->gameObject()->Components().GetComponent<NetworkIdentity>().NetworkId;
		packet.MoveTile = targetTile;
		m_Network->Server().SendPacketToAll(packet);
	}

	void MotionController::JumpToBottomLeftTile(const Tile& targetTile) const
	{
		m_TileMotion->JumpToBottomLeftTile(targetTile);
	}

}