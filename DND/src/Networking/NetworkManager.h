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

	private:
		IdManager<id_t> m_NetworkIdManager;
		IdManager<id_t> m_PlayerIdManager;

		NetworkPlayerInfo m_Player;
		std::unordered_map<SocketAddress, NetworkPlayerInfo> m_OtherPlayers;
		ObjectFactory m_Factory;

		NetworkServer m_Server;

	public:
		NetworkManager();

		NetworkServer& Server();
		ObjectFactory& Factory();

		WelcomePacket Host();
		void Connect(const SocketAddress& address, const ConnectedCallback& callback);
		void Initialize(const WelcomePacket& initInfo);

		id_t GetNextNetworkId() const;
		void SetPlayerId(id_t playerId);
		void SetPlayerPrefab(id_t prefabId);
		void SetPlayer(GameObject* player);
		void IdentifyObject(GameObject* object, id_t networkId, id_t playerId);
		void SetNextAvailableNetworkId(id_t id);

	};

}