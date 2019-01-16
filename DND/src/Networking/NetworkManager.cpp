#include "bltpch.h"
#include "NetworkManager.h"
#include "../Entities/TileTransform.h"
#include "NetworkIdentity.h"

namespace DND
{

	NetworkManager::NetworkManager()
		: m_NetworkIdManager(0, GameObject::InvalidID), m_PlayerIdManager(0, GameObject::InvalidID), m_Player(), m_OtherPlayers(), m_Factory(), m_Server()
	{
	
	}

	NetworkServer& NetworkManager::Server()
	{
		return m_Server;
	}

	ObjectFactory& NetworkManager::Factory()
	{
		return m_Factory;
	}

	WelcomePacket NetworkManager::Host()
	{
		WelcomePacket result;
		result.NetworkId = m_NetworkIdManager.GetNextId();
		result.PlayerId = m_PlayerIdManager.GetNextId();
		m_Server.Initialize(true);
		BLT_CORE_INFO("SERVER STARTED AT {}", m_Server.Address().ToString());
		return result;
	}

	void NetworkManager::Connect(const SocketAddress& address, const NetworkManager::ConnectedCallback& callback)
	{
		m_Server.Initialize(true);
		BLT_CORE_INFO("SERVER STARTED AT {}", m_Server.Address().ToString());
		m_Server.OnWelcomePacket.Clear();
		m_Server.OnWelcomePacket.Subscribe([callback, this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_CORE_INFO("RECEIVED WELCOME PACKET");
			WelcomePacket packet;
			Deserialize(e.Packet, packet);
			callback(packet);
			return true;
		});

		HelloPacket hello;
		m_Server.SendPacket(address, hello);
	}

	void NetworkManager::Initialize(const WelcomePacket& packet)
	{
		m_Server.OnHelloPacket.Clear();
		m_Player.Address = m_Server.Address();
		m_Server.OnHelloPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_CORE_INFO("RECEIVED HELLO PACKET");
			WelcomePacket result;
			result.NetworkId = m_NetworkIdManager.GetNextId();
			result.PlayerId = m_PlayerIdManager.GetNextId();
			PlayerInfo me;
			me.Address = m_Player.Address;
			me.PlayerId = m_Player.PlayerId;
			me.Character.CurrentTile = m_Player.Player->Components().GetComponent<TileTransform>().CurrentTile();
			me.Character.NetworkId = m_Player.Player->Components().GetComponent<NetworkIdentity>().NetworkId;
			me.Character.CharacterPrefabId = m_Player.PrefabId;

			result.Players.push_back(me);
			for (auto& pair : m_OtherPlayers)
			{
				PlayerInfo player;
				player.PlayerId = pair.second.PlayerId;
				player.Address = pair.second.Address;
				player.Character.CurrentTile = pair.second.Player->Components().GetComponent<TileTransform>().CurrentTile();
				player.Character.NetworkId = pair.second.Player->Components().GetComponent<NetworkIdentity>().NetworkId;
				player.Character.CharacterPrefabId = pair.second.PrefabId;
				result.Players.push_back(player);
			}

			e.Server->SendPacket(e.FromAddress, result);
			return true;
		});
	}

	id_t NetworkManager::GetNextNetworkId() const
	{
		return m_NetworkIdManager.GetNextId();
	}

	void NetworkManager::SetPlayerId(id_t playerId)
	{
		m_Player.PlayerId = playerId;
	}

	void NetworkManager::SetPlayerPrefab(id_t prefabId)
	{
		m_Player.PrefabId = prefabId;
	}

	void NetworkManager::SetPlayer(GameObject* player)
	{
		m_Player.Player = player;
	}

	void NetworkManager::IdentifyObject(GameObject* object, id_t networkId, id_t playerId)
	{
		object->Components().AddComponent<NetworkIdentity>(networkId, playerId);
	}

	void NetworkManager::SetNextAvailableNetworkId(id_t id)
	{
		m_NetworkIdManager.SetNextAvailableId(id);
	}

}