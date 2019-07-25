#pragma once
#include "PlayerNetworkData.h"
#include "Player.h"
#include "Characters/NetworkObjectManager.h"

namespace DND
{

	class PlayerManager
	{
	private:
		NetworkObjectManager* m_ObjectManager;

		IdManager<id_t> m_PlayerIdManager;
		Player m_LocalPlayer;
		std::unordered_map<id_t, Player> m_OtherPlayers;

	public:
		PlayerManager(NetworkObjectManager& objectManager);

		const IdManager<id_t>& PlayerIds() const;
		IdManager<id_t>& PlayerIds();
		bool HasPlayerId(id_t playerId) const;
		const Player& LocalPlayer() const;
		const Player& GetPlayer(id_t playerId) const;
		std::vector<Player> OtherPlayers() const;
		std::vector<Player> AllPlayers() const;

		void SetLocalPlayer(Player player);
		void AddPlayer(Player player);
		void RemovePlayer(id_t playerId);
		void ClearOtherPlayers();
		void ClearAllPlayers();

		void AddPlayer(const PlayerNetworkData& player);
		void SetLocalPlayer(const PlayerNetworkData& player);
		PlayerNetworkData GetPlayerData(id_t playerId) const;
		PlayerNetworkData GetLocalPlayerData() const;
		std::vector<PlayerNetworkData> GetOtherPlayerData() const;
		std::vector<PlayerNetworkData> GetAllPlayerData() const;

	private:
		PlayerNetworkData GetPlayerNetworkData(const Player& player) const;

	};

}