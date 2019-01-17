#include "bltpch.h"
#include "PlayerController.h"
#include "../GameManager.h"
#include "../Networking/NetworkIdentity.h"

namespace DND
{

	PlayerController::PlayerController() : Component(),
		m_Left(Keycode::A), m_Up(Keycode::W), m_Right(Keycode::D), m_Down(Keycode::S)
	{
	
	}

	void PlayerController::Update()
	{
		TileTransform& t = gameObject()->Components().GetComponent<TileTransform>();
		TileMotion& m = gameObject()->Components().GetComponent<TileMotion>();
		if (!m.IsMoving())
		{
			Tile moveToTile = Tile(-1, -1);
			if (Input::KeyDown(m_Up))
			{
				moveToTile = t.CurrentTile() + Tile(0, 1);
				m.SetTargetTile(moveToTile);
			}
			else if (Input::KeyDown(m_Down))
			{
				moveToTile = t.CurrentTile() + Tile(0, -1);
				m.SetTargetTile(moveToTile);
			}
			else if (Input::KeyDown(m_Left))
			{
				moveToTile = t.CurrentTile() + Tile(-1, 0);
				m.SetTargetTile(moveToTile);
			}
			else if (Input::KeyDown(m_Right))
			{
				moveToTile = t.CurrentTile() + Tile(1, 0);
				m.SetTargetTile(moveToTile);
			}

			if (moveToTile.x >= 0 && moveToTile.y >= 0)
			{
				PlayerMovePacket packet;
				packet.NetworkId = gameObject()->Components().GetComponent<NetworkIdentity>().NetworkId;
				packet.MoveToTile = moveToTile;
				GameManager::Get().Network().SendPacketToAll(packet);
			}
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		return std::unique_ptr<PlayerController>();
	}

}