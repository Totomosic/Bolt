#include "bltpch.h"
#include "PlayerController.h"
#include "../GameManager.h"
#include "../Networking/NetworkIdentity.h"
#include "../Spells/SpellCaster.h"

namespace DND
{

	PlayerController::PlayerController() : Component(),
		m_Left(Keycode::A), m_Up(Keycode::W), m_Right(Keycode::D), m_Down(Keycode::S), m_LastPressed(Keycode::Left), m_Actions(), m_CanMove(true)
	{
	
	}

	void PlayerController::Update()
	{
		TileTransform& t = gameObject()->Components().GetComponent<TileTransform>();
		TileMotion& m = gameObject()->Components().GetComponent<TileMotion>();
		m_LastPressed = TestPressedKeys();
		Tile tileDiff = Tile(0, 0);
		if (!m.IsMoving())
		{
			if (Input::KeyDown(m_LastPressed))
			{
				tileDiff = GetTileOffset(m_LastPressed);
			}
			else
			{
				if (Input::KeyDown(m_Left))
				{
					tileDiff = GetTileOffset(m_Left);
				}
				if (Input::KeyDown(m_Right))
				{
					tileDiff = GetTileOffset(m_Right);
				}
				if (Input::KeyDown(m_Up))
				{
					tileDiff = GetTileOffset(m_Up);
				}
				if (Input::KeyDown(m_Down))
				{
					tileDiff = GetTileOffset(m_Down);
				}
			}
			if (tileDiff.x != 0 || tileDiff.y != 0)
			{
				Tile moveToTile = t.CurrentTile() + tileDiff;
				QueueAction([moveToTile, &m](GameObject* player)
				{
					m.SetTargetTile(moveToTile);
					PlayerMovePacket packet;
					packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
					packet.MoveToTile = moveToTile;
					GameManager::Get().Network().SendPacketToAll(packet);
				});
			}
		}		

		if (Input::KeyPressed(Keycode::Space))
		{
			QueueAction([this](GameObject* player)
			{
				CastSpellPacket packet;
				packet.CasterNetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
				packet.SpellId = 0;
				packet.SpellData = GameManager::Get().Spells().GetSpell(packet.SpellId).CreateFunc(player, GameManager::Get());

				GameManager::Get().Network().SendPacketToAll(packet);
				player->Components().GetComponent<SpellCaster>().Cast(packet.SpellId, packet.SpellData);

				Freeze();
				PlayerController* c = this;
				Time::RenderingTimeline().AddFunction(GameManager::Get().Spells().GetSpell(packet.SpellId).CastTime, [c]()
				{
					c->Unfreeze();
				});
			});
		}

		while (!IsFrozen() && !m_Actions.empty())
		{
			m_Actions.front()(gameObject());
			m_Actions.pop_front();
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		return std::unique_ptr<PlayerController>();
	}

	Keycode PlayerController::TestPressedKeys() const
	{
		Keycode result = m_LastPressed;
		if (Input::KeyPressed(m_Left))
		{
			result = m_Left;
		}
		if (Input::KeyPressed(m_Right))
		{
			result = m_Right;
		}
		if (Input::KeyPressed(m_Up))
		{
			result = m_Up;
		}
		if (Input::KeyPressed(m_Down))
		{
			result = m_Down;
		}
		return result;
	}

	Tile PlayerController::GetTileOffset(Keycode code) const
	{
		if (m_Left == code)
		{
			return { -1, 0 };
		}
		if (m_Right == code)
		{
			return { 1, 0 };
		}
		if (m_Up == code)
		{
			return { 0, 1 };
		}
		if (m_Down == code)
		{
			return { 0, -1 };
		}
		return Tile();
	}

	void PlayerController::QueueAction(const PlayerController::PlayerAction& action)
	{
		if (m_Actions.size() < PlayerController::MAX_QUEUED_ACTIONS)
		{
			m_Actions.push_back(action);
		}
	}

}