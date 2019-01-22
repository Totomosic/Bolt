#include "bltpch.h"
#include "NetworkManager.h"
#include "../Entities/TileTransform.h"
#include "NetworkController.h"
#include "NetworkIdentity.h"
#include "../Spells/SpellCaster.h"

#include "../GameManager.h"

namespace DND
{

	NetworkManager::NetworkManager()
		: m_NetworkIdManager(0, GameObject::InvalidID), m_PlayerIdManager(0, GameObject::InvalidID), m_Player(), m_OtherPlayers(), m_Factory(), m_NetworkObjects(), m_Server()
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

	GameObject* NetworkManager::GetObjectByNetworkId(id_t networkId) const
	{
		return m_NetworkObjects.at(networkId);
	}

	NetworkPlayerInfo& NetworkManager::GetPlayerById(id_t playerId)
	{
		if (playerId == m_Player.PlayerId)
		{
			return m_Player;
		}
		for (auto& pair : m_OtherPlayers)
		{
			if (pair.second.PlayerId == playerId)
			{
				return pair.second;
			}
		}
		BLT_ASSERT(false, "No player with id {}", playerId);
		return *(NetworkPlayerInfo*)nullptr;
	}

	WelcomePacket NetworkManager::Host()
	{
		WelcomePacket result;
		result.NetworkId = m_NetworkIdManager.GetNextId();
		result.PlayerId = m_PlayerIdManager.GetNextId();
		result.Address = m_Server.Address();
		return result;
	}

	void NetworkManager::Connect(const SocketAddress& address, const NetworkManager::ConnectedCallback& callback)
	{
		m_Server.OnWelcomePacket.Clear();
		m_Server.OnWelcomePacket.Subscribe([callback, this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED WELCOME PACKET");
			WelcomePacket packet;
			Deserialize(e.Packet, packet);
			callback(packet);
			return true;
		});
		HelloPacket hello;
		m_Server.SendPacket(address, hello);
		BLT_INFO("CONNECTING TO {}", address.ToString());
	}

	void NetworkManager::Initialize()
	{
		m_Player.Address = m_Server.Address();

		m_Server.OnClientConnectingPacket.Clear();
		m_Server.OnClientConnectingPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED CLIENT CONNECTING PACKET");
			ClientConnectingPacket packet;
			Deserialize(e.Packet, packet);
			BLT_INFO("SENT IGNORE PACKET TO {}", packet.Client.ToString());
			BLT_ASSERT(packet.Client.m_SockAddr.sa_family == AF_INET, "NOT INET ADDRESS");
			IgnorePacket ignore;
			Server().SendPacket(packet.Client, ignore);
			return true;
		});

		m_Server.OnHelloPacket.Clear();
		m_Server.OnHelloPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED HELLO PACKET");
			BLT_INFO("INCOMING CONNECTION FROM {}", e.FromAddress.ToString());
			WelcomePacket result;
			result.NetworkId = GetNextNetworkId();
			result.PlayerId = GetNextPlayerId();
			result.Address = e.FromAddress;
			PlayerInfo me;
			me.Address = m_Player.Address;
			me.PlayerId = m_Player.PlayerId;
			me.Character.CurrentTile = m_Player.Player->Components().GetComponent<TileTransform>().CurrentTile();
			me.Character.NetworkId = m_Player.Player->Components().GetComponent<NetworkIdentity>().NetworkId;
			me.Character.CharacterPrefabId = m_Player.PrefabId;
			me.Character.Stats = m_Player.Player->Components().GetComponent<StatsComponent>().Stats();

			result.Players.push_back(me);
			for (auto& pair : m_OtherPlayers)
			{
				PlayerInfo player;
				player.PlayerId = pair.second.PlayerId;
				player.Address = pair.second.Address;
				player.Character.CurrentTile = pair.second.Player->Components().GetComponent<TileTransform>().CurrentTile();
				player.Character.NetworkId = pair.second.Player->Components().GetComponent<NetworkIdentity>().NetworkId;
				player.Character.Stats = pair.second.Player->Components().GetComponent<StatsComponent>().Stats();
				player.Character.CharacterPrefabId = pair.second.PrefabId;
				result.Players.push_back(player);
			}

			e.Server->SendPacket(e.FromAddress, result);
			return true;
		});

		m_Server.OnIntroductionPacket.Clear();
		m_Server.OnIntroductionPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED INTRODUCTION PACKET");
			IntroductionPacket packet;
			Deserialize(e.Packet, packet);
			GameObject* newPlayer = Factory().Instantiate(Factory().GetPrefab(packet.Character.CharacterPrefabId));
			newPlayer->Components().GetComponent<TileTransform>().SetCurrentTile(packet.Character.CurrentTile, true);
			newPlayer->Components().GetComponent<StatsComponent>().SetStats(packet.Character.Stats);

			NetworkPlayerInfo pl;
			pl.Address = e.FromAddress;
			pl.PlayerId = packet.PlayerId;
			pl.PrefabId = packet.Character.CharacterPrefabId;
			pl.Player = newPlayer;
			AddOtherPlayer(pl);

			IdentifyObject(newPlayer, packet.Character.NetworkId, packet.PlayerId);
			MakeNetworkPlayer(newPlayer);

			SetNextAvailableNetworkId(packet.Character.NetworkId + 1);
			SetNextAvailablePlayerId(packet.PlayerId + 1);
			return true;
		});

		m_Server.OnDisconnectPacket.Clear();
		m_Server.OnDisconnectPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED DISCONNECT PACKET");
			DisconnectPlayer(e.FromAddress);
			return true;
		});

		m_Server.OnPlayerMovePacket.Clear();
		m_Server.OnPlayerMovePacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED PLAYER MOVE PACKET");
			PlayerMovePacket packet;
			Deserialize(e.Packet, packet);
			GameObject* obj = GetObjectByNetworkId(packet.NetworkId);
			obj->Components().GetComponent<NetworkController>().OnPlayerMovePacket(packet);
			return true;
		});

		m_Server.OnCastSpellPacket.Clear();
		m_Server.OnCastSpellPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED CAST SPELL PACKET");
			CastSpellPacket packet;
			Deserialize(e.Packet, packet);
			GameObject* caster = GetObjectByNetworkId(packet.CasterNetworkId);
			caster->Components().GetComponent<SpellCaster>().Cast(packet.SpellId, packet.SpellData, GameManager::Get().GetStateObjects());
			return true;
		});

		m_Server.OnStatUpdatePacket.Clear();
		m_Server.OnStatUpdatePacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED STAT UPDATE PACKET");
			StatUpdatePacket packet;
			Deserialize(e.Packet, packet);
			GameObject* object = GetObjectByNetworkId(packet.NetworkId);
			object->Components().GetComponent<StatsComponent>().SetStats(packet.Stats);
			return true;
		});

		m_Server.OnDeathPacket.Clear();
		m_Server.OnDeathPacket.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			BLT_INFO("RECEIVED DEATH PACKET");
			DeathPacket packet;
			Deserialize(e.Packet, packet);
			return true;
		});

		m_Server.Initialize(true);
	}

	void NetworkManager::Exit(const std::function<void()>& onExit)
	{
		m_Server.OnShutdown.Clear();
		m_Server.OnShutdown.Subscribe([this, onExit](id_t listenerId, ServerShutdownEvent& e)
		{
			BLT_INFO("SUCCESSFULLY SHUTDOWN SERVER");
			std::unordered_map<SocketAddress, NetworkPlayerInfo> players = m_OtherPlayers;
			for (auto& pair : players)
			{
				DisconnectPlayer(pair.first);
			}
			m_OtherPlayers.clear();
			m_NetworkIdManager.Reset();
			m_PlayerIdManager.Reset();
			onExit();
			return true;
		});
		m_Server.Terminate();		
	}

	id_t NetworkManager::GetNextNetworkId() const
	{
		return m_NetworkIdManager.GetNextId();
	}
	
	id_t NetworkManager::GetNextPlayerId() const
	{
		return m_PlayerIdManager.GetNextId();
	}

	void NetworkManager::SetPlayer(const NetworkPlayerInfo& player)
	{
		m_Player = player;
	}

	void NetworkManager::IdentifyObject(GameObject* object, id_t networkId, id_t playerId)
	{
		object->Components().AddComponent<NetworkIdentity>(networkId, playerId);
		m_NetworkObjects[networkId] = object;
		GetPlayerById(playerId).OwnedObjects.push_back(object);
	}

	void NetworkManager::MakeNetworkPlayer(GameObject* player)
	{
		player->Components().AddComponent<NetworkController>();
	}

	void NetworkManager::SetNextAvailableNetworkId(id_t id)
	{
		m_NetworkIdManager.SetNextAvailableId(id);
	}

	void NetworkManager::SetNextAvailablePlayerId(id_t id)
	{
		m_PlayerIdManager.SetNextAvailableId(id);
	}

	void NetworkManager::AddOtherPlayer(const NetworkPlayerInfo& player)
	{
		m_OtherPlayers[player.Address] = player;
	}

	void NetworkManager::DisconnectPlayer(const SocketAddress& address)
	{
		if (m_OtherPlayers.find(address) != m_OtherPlayers.end())
		{
			NetworkPlayerInfo& player = m_OtherPlayers.at(address);
			for (GameObject* object : player.OwnedObjects)
			{
				Destroy(object);
			}
			m_OtherPlayers.erase(address);
		}
	}
}