#pragma once
#include "Networking/NetworkManager.h"

namespace DND
{

	class PlayerManager
	{
	public:
		struct PlayerCharacterInfo
		{
		public:
			id_t NetworkId;
			GameObject* PlayerObject;
		};

		struct PlayerInstance
		{
		public:
			id_t PlayerId;
			PlayerCharacterInfo Player;
			id_t ConnectionId;
		};

	private:
		IdManager<id_t> m_PlayerIdManager;
		PlayerInstance m_LocalPlayer;
		std::unordered_map<id_t, PlayerInstance> m_OtherPlayers;

	public:
		PlayerManager();

		id_t GetNextPlayerId() const;
		id_t PeekNextPlayerId() const;
		void SetNextAvailablePlayerId(id_t playerId);

		const PlayerInstance& LocalPlayer() const;
		GameObject* LocalPlayerObject() const;
		void SetLocalPlayer(id_t playerId, PlayerCharacterInfo character);

		bool HasPlayer(id_t playerId) const;
		const PlayerInstance& GetPlayer(id_t playerId) const;
		const std::unordered_map<id_t, PlayerInstance>& OtherPlayers() const;

		id_t AddPlayer(id_t playerId, const PlayerCharacterInfo& player, id_t connectionId);
		void RemovePlayer(id_t playerId);
		void Clear();

	};

}