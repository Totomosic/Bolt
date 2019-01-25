#include "bltpch.h"
#include "PlayerManager.h"

namespace DND
{

	PlayerManager::PlayerManager()
		: m_PlayerIdManager(0, GameObject::InvalidID - 1), m_LocalPlayer(), m_LocalPlayerObject(nullptr), m_OtherPlayers()
	{
	
	}

	id_t PlayerManager::GetNextPlayerId() const
	{
		return m_PlayerIdManager.GetNextId();
	}

	id_t PlayerManager::PeekNextPlayerId() const
	{
		return m_PlayerIdManager.PeekNextId();
	}

	void PlayerManager::SetNextAvailablePlayerId(id_t playerId)
	{
		m_PlayerIdManager.SetNextAvailableId(playerId);
	}

	bool PlayerManager::HasPlayer(id_t playerId) const
	{
		return m_OtherPlayers.find(playerId) != m_OtherPlayers.end();
	}

	const PlayerManager::PlayerInfo& PlayerManager::LocalPlayer() const
	{
		return m_LocalPlayer;
	}

	GameObject* PlayerManager::LocalPlayerObject() const
	{
		return m_LocalPlayerObject;
	}

	const PlayerManager::PlayerInstance& PlayerManager::GetPlayer(id_t playerId) const
	{
		return m_OtherPlayers.at(playerId);
	}

	void PlayerManager::SetLocalPlayer(PlayerInfo player, GameObject* object)
	{
		m_LocalPlayer = std::move(player);
		BLT_CORE_INFO("PLAYER ID = {0} NETWORK ID = {1}", m_LocalPlayer.PlayerId, m_LocalPlayer.Character.NetworkId);
		m_LocalPlayerObject = object;
	}

	const std::unordered_map<id_t, PlayerManager::PlayerInstance>& PlayerManager::OtherPlayers() const
	{
		return m_OtherPlayers;
	}

	id_t PlayerManager::AddPlayer(id_t playerId, const PlayerInfo& player, GameObject* object, id_t connectionId)
	{
		m_OtherPlayers[playerId] = { player, connectionId, object };
		return playerId;
	}

	void PlayerManager::RemovePlayer(id_t playerId)
	{
		m_OtherPlayers.erase(playerId);
		m_PlayerIdManager.ReleaseId(playerId);
	}

	void PlayerManager::Clear()
	{
		m_OtherPlayers.clear();
		m_PlayerIdManager.Reset();
		m_LocalPlayerObject = nullptr;
	}

}