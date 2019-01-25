#include "bltpch.h"
#include "PlayerController.h"
#include "../GameManager.h"
#include "../Networking/NetworkIdentity.h"
#include "../Spells/SpellCaster.h"

#include "../Networking/GamePlayPacketSerialization.h"

namespace DND
{

	PlayerController::PlayerController() : Component(),
		m_Left(Keycode::A), m_Up(Keycode::W), m_Right(Keycode::D), m_Down(Keycode::S), m_LastPressed(Keycode::Left), m_Spells(), m_Actions(), m_CanMove(true)
	{
		id_t fireball = m_Spells.AddSpell(1);
		id_t teleport = m_Spells.AddSpell(0);
		m_SpellKeyMap[Keycode::Q] = fireball;
		m_SpellKeyMap[Keycode::E] = teleport;

	}

	void PlayerController::FreezeFor(float seconds)
	{
		if (seconds > 0.0f)
		{
			GameManager::Get().AddActiveTimer(&Time::RenderingTimeline().AddFunction(seconds, [this]()
			{
				Unfreeze();
			}));
			Freeze();
		}
	}

	void PlayerController::Update()
	{
		Spells().Update();

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
				QueueAction([this, tileDiff, &m, &t](GameObject* player)
				{
					if (IsFrozen())
					{
						return false;
					}
					Tile moveToTile = t.CurrentTile() + tileDiff;
					m.SetTargetTile(moveToTile);
					EntityMovedPacket packet;
					packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
					packet.MoveToTile = moveToTile;
					GameManager::Get().Network().SendPacketToAll(packet);
					return true;
				});
			}
		}

		for (auto pair : m_SpellKeyMap)
		{
			if (Input::KeyPressed(pair.first))
			{
				GameState state = GameManager::Get().GetGameState();
				QueueAction([this, pair, state](GameObject* player)
				{
					if (Spells().CanCast(pair.second))
					{
						if (IsFrozen())
						{
							return false;
						}
						std::unique_ptr<SpellInstance> spellInstance = Spells().GetSpell(pair.second)->CreateInstance(gameObject(), state);
						SpellInstance::SpellCastResult result = Spells().CastSpell(pair.second, spellInstance, player, GameManager::Get().GetStateObjects());
						FreezeFor(result.CastTime);
					}
					return true;
				});
			}
		}

		if (!m_Actions.empty() && m_Actions.front()(gameObject()))
		{
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

	void PlayerController::QueueAction(PlayerController::PlayerAction&& action)
	{
		if (m_Actions.size() < PlayerController::MAX_QUEUED_ACTIONS)
		{
			m_Actions.push_back(std::move(action));
		}
	}

}