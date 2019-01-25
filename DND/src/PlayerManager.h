#pragma once
#include "Networking/NetworkManager.h"

namespace DND
{

	class PlayerManager
	{
	public:
		struct PlayerInfo
		{
		public:
			id_t PlayerId;
			CharacterInfo Character;
		};

		struct PlayerInstance
		{
		public:
			PlayerInfo Player;
			id_t ConnectionId;
			GameObject* PlayerObject;
		};

	private:
		IdManager<id_t> m_PlayerIdManager;
		PlayerInfo m_LocalPlayer;
		GameObject* m_LocalPlayerObject;
		std::unordered_map<id_t, PlayerInstance> m_OtherPlayers;

	public:
		PlayerManager();

		id_t GetNextPlayerId() const;
		id_t PeekNextPlayerId() const;
		void SetNextAvailablePlayerId(id_t playerId);

		bool HasPlayer(id_t playerId) const;
		const PlayerInfo& LocalPlayer() const;
		GameObject* LocalPlayerObject() const;
		void SetLocalPlayer(PlayerInfo player, GameObject* object);
		const PlayerInstance& GetPlayer(id_t playerId) const;
		const std::unordered_map<id_t, PlayerInstance>& OtherPlayers() const;

		id_t AddPlayer(id_t playerId, const PlayerInfo& player, GameObject* object, id_t connectionId);
		void RemovePlayer(id_t playerId);
		void Clear();

	};

}