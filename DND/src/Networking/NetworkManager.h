#pragma once
#include "Packets.h"
#include "NetworkServer.h"

namespace DND
{

	struct NetworkPlayerInfo
	{
	public:
		SocketAddress Address;
		id_t PlayerId;
		id_t PrefabId;
		GameObject* Player;
		std::vector<GameObject*> OwnedObjects;
	};

	class NetworkManager
	{
	public:
		using ConnectedCallback = std::function<void(WelcomePacket)>;

		static constexpr id_t InvalidId = (id_t)-1;

	private:
		IdManager<id_t> m_NetworkIdManager;
		IdManager<id_t> m_PlayerIdManager;

		NetworkPlayerInfo m_Player;
		std::unordered_map<SocketAddress, NetworkPlayerInfo> m_OtherPlayers;
		ObjectFactory m_Factory;
		std::unordered_map<id_t, GameObject*> m_NetworkObjects;

		NetworkServer m_Server;

	public:
		NetworkManager();

		NetworkServer& Server();
		ObjectFactory& Factory();
		GameObject* GetObjectByNetworkId(id_t networkId) const;
		NetworkPlayerInfo& GetPlayerById(id_t playerId);

		WelcomePacket Host();
		void Connect(const SocketAddress& address, const ConnectedCallback& callback);
		void Initialize(const WelcomePacket& initInfo);
		void Exit();
		void SetAddress(const SocketAddress& address);

		id_t GetNextNetworkId() const;
		id_t GetNextPlayerId() const;
		void SetPlayerId(id_t playerId);
		void SetPlayerPrefab(id_t prefabId);
		void SetPlayer(GameObject* player);
		void IdentifyObject(GameObject* object, id_t networkId, id_t playerId);
		void MakeNetworkPlayer(GameObject* player);
		void SetNextAvailableNetworkId(id_t id);
		void SetNextAvailablePlayerId(id_t id);
		void AddOtherPlayer(const NetworkPlayerInfo& player);

		void DisconnectPlayer(const SocketAddress& address);

		template<typename T>
		void SendPacketToAll(const T& packet)
		{
			for (auto& pair : m_OtherPlayers)
			{
				Server().SendPacket(pair.second.Address, packet);
			}
		}

	};

}