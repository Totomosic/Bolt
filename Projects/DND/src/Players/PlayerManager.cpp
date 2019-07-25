#include "bltpch.h"
#include "PlayerManager.h"

#include "Characters/Controllers/PlayerController.h"

namespace DND
{

	PlayerManager::PlayerManager(NetworkObjectManager& objectManager)
		: m_ObjectManager(&objectManager), m_PlayerIdManager(0, GameObject::InvalidID - 1), m_LocalPlayer(), m_OtherPlayers()
	{
		
	}

	const IdManager<id_t>& PlayerManager::PlayerIds() const
	{
		return m_PlayerIdManager;
	}

	IdManager<id_t>& PlayerManager::PlayerIds()
	{
		return m_PlayerIdManager;
	}

	bool PlayerManager::HasPlayerId(id_t playerId) const
	{
		return m_OtherPlayers.find(playerId) != m_OtherPlayers.end();
	}

	const Player& PlayerManager::LocalPlayer() const
	{
		return m_LocalPlayer;
	}

	const Player& PlayerManager::GetPlayer(id_t playerId) const
	{
		return m_OtherPlayers.at(playerId);
	}

	std::vector<Player> PlayerManager::OtherPlayers() const
	{
		std::vector<Player> result;
		for (const auto& pair : m_OtherPlayers)
		{
			result.push_back(pair.second);
		}
		return result;
	}

	std::vector<Player> PlayerManager::AllPlayers() const
	{
		std::vector<Player> result = OtherPlayers();
		result.push_back(LocalPlayer());
		return result;
	}

	void PlayerManager::SetLocalPlayer(Player player)
	{
		m_LocalPlayer = std::move(player);
	}

	void PlayerManager::AddPlayer(Player player)
	{
		m_OtherPlayers[player.PlayerId()] = std::move(player);
	}

	void PlayerManager::RemovePlayer(id_t playerId)
	{
		Player& player = m_OtherPlayers.at(playerId);
		m_ObjectManager->RemoveObject(player.Object());
		m_OtherPlayers.erase(playerId);
	}

	void PlayerManager::ClearOtherPlayers()
	{
		for (auto& pair : m_OtherPlayers)
		{
			m_ObjectManager->RemoveObject(pair.second.Object());
		}
		m_OtherPlayers.clear();
	}

	void PlayerManager::ClearAllPlayers()
	{
		ClearOtherPlayers();
		m_ObjectManager->RemoveObject(LocalPlayer().Object());
	}

	void PlayerManager::AddPlayer(const PlayerNetworkData& data)
	{
		GameObject* player = m_ObjectManager->CreateFromNetworkData(data.PlayerObject);
		Player p(data.Address, data.PlayerId, player);
		AddPlayer(std::move(p));
	}

	void PlayerManager::SetLocalPlayer(const PlayerNetworkData& data)
	{
		GameObject* player = m_ObjectManager->CreateFromNetworkData(data.PlayerObject);
		Player p(data.Address, data.PlayerId, player);
		SetLocalPlayer(std::move(p));
		player->Components().AddComponent<PlayerController>();
	}

	PlayerNetworkData PlayerManager::GetPlayerData(id_t playerId) const
	{
		return GetPlayerNetworkData(GetPlayer(playerId));
	}

	PlayerNetworkData PlayerManager::GetLocalPlayerData() const
	{
		return GetPlayerNetworkData(LocalPlayer());
	}

	std::vector<PlayerNetworkData> PlayerManager::GetOtherPlayerData() const
	{
		std::vector<PlayerNetworkData> result;
		for (Player& player : OtherPlayers())
		{
			PlayerNetworkData data = GetPlayerNetworkData(player);
			result.push_back(data);
		}
		return result;
	}

	std::vector<PlayerNetworkData> PlayerManager::GetAllPlayerData() const
	{
		std::vector<PlayerNetworkData> result = GetOtherPlayerData();
		PlayerNetworkData data = GetLocalPlayerData();
		result.push_back(data);
		return result;
	}

	PlayerNetworkData PlayerManager::GetPlayerNetworkData(const Player& player) const
	{
		PlayerNetworkData data;
		data.Address = player.Address();
		data.PlayerId = player.PlayerId();
		data.PlayerObject = m_ObjectManager->GetEntityData(player.Object());
		return data;
	}

}