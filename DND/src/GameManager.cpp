#include "bltpch.h"
#include "GameManager.h"

#include "Camera/PlayerCamera.h"
#include "Entities/TileTransform.h"
#include "Networking/NetworkController.h"

#include "Networking/GamePlayPacketSerialization.h"

namespace DND
{

	GameManager* GameManager::s_Instance = nullptr;

	GameManager& GameManager::Get()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new GameManager(SceneManager::GetSceneByName("Game").GetLayer(0));
		}
		return *s_Instance;
	}

	void GameManager::Terminate()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	GameManager::GameManager(Layer& layer)
		: m_LocalCamera(nullptr), m_Tilemap(layer, TILEMAP_WIDTH, TILEMAP_HEIGHT, TILE_WIDTH, TILE_HEIGHT), m_Prefabs(), m_Factory(layer), m_Network(), m_Spells(), m_ActiveFunctions(), m_ActiveTimers()
	{
	
	}

	void GameManager::Host(PlayerCharacterInfo player, GameManager::LoadGameCallback callback)
	{
		WelcomePacket packet;
		packet.NetworkId = Network().Objects().GetNextNetworkId();
		packet.PlayerId = Players().GetNextPlayerId();
		packet.NextNetworkId = packet.NetworkId + 1;
		packet.NextPlayerId = packet.PlayerId + 1;
		packet.OtherCharacters;
		packet.Players;
		InitializeListeners();
		callback(packet, std::move(player));
	}

	void GameManager::Join(id_t connectionId, PlayerCharacterInfo player, GameManager::LoadGameCallback callback)
	{
		const SocketAddress& address = Network().Connections().GetRoutableAddress(connectionId);
		
		Network().Server().AddTemporaryPacketListener(PacketType::Welcome, [player = std::move(player), callback = std::move(callback)](ReceivedPacketEvent& e)
		{
			BLT_CORE_INFO("RECEIVED WELCOME PACKET FROM {}", e.FromAddress.ToString());
			WelcomePacket packet;
			Deserialize(e.Packet, packet);
			callback(packet, std::move(player));
			return true;
		}, 1);

		InitializeListeners();

		HelloPacket packet;
		BLT_CORE_INFO("SENDING HELLO PACKET TO {}", address.ToString());
		Network().Server().SendPacket(address, packet);
	}

	void GameManager::Initialize()
	{

	}

	void GameManager::Exit()
	{
		Exit([]() {});
	}

	void GameManager::Exit(const std::function<void()>& callback)
	{
		PlayerDisconnectPacket packet;
		packet.PlayerId = Players().LocalPlayer().PlayerId;
		Network().SendPacketToAll(packet);
		Network().Exit([this, callback]()
		{
			for (Timer* func : m_ActiveFunctions)
			{
				Time::RenderingTimeline().RemoveFunction(func);
			}
			for (Timer* timer : m_ActiveTimers)
			{
				Time::RenderingTimeline().RemoveTimer(timer);
			}
			SceneManager::SetCurrentSceneByName("Title");
			callback();
		});
		Players().Clear();
		Network().Objects().Clear();
		Network().m_Connections.ClearConnections();
	}

	void GameManager::Update()
	{

	}

	Camera* GameManager::LocalCamera() const
	{
		return m_LocalCamera;
	}

	void GameManager::SetLocalCamera(Camera* camera)
	{
		m_LocalCamera = camera;
	}

	GameStateObjects GameManager::GetStateObjects()
	{
		return { &Network(), &Factory(), &GetTilemap(), &Players() };
	}

	GameState GameManager::GetGameState()
	{
		GameState state = { GetStateObjects() };
		state.SelectedTile = CurrentlySelectedTile();
		return state;
	}

	Tile GameManager::CurrentlySelectedTile() const
	{
		BLT_ASSERT(m_LocalCamera != nullptr, "No local camera");
		return m_LocalCamera->Components().GetComponent<PlayerCamera>().SelectedTile();
	}

	PlayerManager& GameManager::Players()
	{
		return m_Players;
	}

	PrefabList& GameManager::Prefabs()
	{
		return m_Prefabs;
	}

	ObjectFactory& GameManager::Factory()
	{
		return m_Factory;
	}

	Tilemap& GameManager::GetTilemap()
	{
		return m_Tilemap;
	}

	NetworkManager& GameManager::Network()
	{
		return m_Network;
	}

	SpellList& GameManager::Spells()
	{
		return m_Spells;
	}

	void GameManager::AddActiveFunction(Timer* function)
	{
		m_ActiveFunctions.push_back(function);
	}

	void GameManager::AddActiveTimer(Timer* timer)
	{
		m_ActiveTimers.push_back(timer);
	}

	void GameManager::InitializeListeners()
	{
		Network().Server().AddPacketListener(PacketType::Hello, [this](ReceivedPacketEvent& e)
		{
			WelcomePacket packet;
			packet.PlayerId = Players().GetNextPlayerId();
			packet.NetworkId = Network().Objects().GetNextNetworkId();
			packet.NextPlayerId = packet.NextPlayerId + 1;
			packet.NextNetworkId = packet.NextNetworkId + 1;
			
			NetworkPlayerInfo me;
			me.Character = Players().LocalPlayer().Character;
			me.Character.CurrentTile = Players().LocalPlayerObject()->Components().GetComponent<TileTransform>().CurrentTile();
			me.PlayerId = Players().LocalPlayer().PlayerId;
			me.Connection.Address = Network().Address();
			packet.Players.push_back(me);

			for (const auto& pair : Players().OtherPlayers())
			{
				NetworkPlayerInfo player;
				player.PlayerId = pair.second.Player.PlayerId;
				player.Character = pair.second.Player.Character;
				player.Character.CurrentTile = pair.second.PlayerObject->Components().GetComponent<TileTransform>().CurrentTile();
				player.Connection.Address = Network().Connections().GetAddressPair(pair.second.ConnectionId);
				packet.Players.push_back(player);
			}

			Network().Server().SendPacket(e.FromAddress, packet);
			return true;
		});

		Network().Server().AddPacketListener(PacketType::Introduction, [this](ReceivedPacketEvent& e)
		{
			IntroductionPacket packet;
			Deserialize(e.Packet, packet);
			
			PlayerManager::PlayerInfo player;
			player.Character = packet.Player.Character;
			player.PlayerId = packet.Player.PlayerId;
			id_t connectionId = Network().Connections().GetConnectionId(packet.Player.Connection.Address);

			GameObject* object = Factory().Instantiate(Factory().GetPrefab(player.Character.PrefabId));
			object->Components().AddComponent<NetworkController>();
			object->Components().GetComponent<TileTransform>().SetCurrentTile(player.Character.CurrentTile, true);

			Network().Objects().IdentifyObject(player.Character.NetworkId, object);
			Players().AddPlayer(player.PlayerId, player, object, connectionId);

			return true;
		});

		Network().Server().AddPacketListener(PacketType::PlayerDisconnect, [this](ReceivedPacketEvent& e)
		{
			PlayerDisconnectPacket packet;
			Deserialize(e.Packet, packet);
			id_t connectionId = Players().GetPlayer(packet.PlayerId).ConnectionId;
			Network().m_Connections.RemoveConnection(connectionId);
			GameObject* playerObject = Players().GetPlayer(packet.PlayerId).PlayerObject;
			id_t networkId = playerObject->Components().GetComponent<NetworkIdentity>().NetworkId;
			Network().Objects().ReleaseObject(networkId);
			Destroy(playerObject);
			Players().RemovePlayer(packet.PlayerId);
			return true;
		});

		Network().Server().AddPacketListener(PacketType::EntityMoved, [this](ReceivedPacketEvent& e)
		{
			EntityMovedPacket packet;
			Deserialize(e.Packet, packet);
			GameObject* object = Network().Objects().GetObjectByNetworkId(packet.NetworkId);
			object->Components().GetComponent<NetworkController>().MoveToTile(packet.MoveToTile);
			return true;
		});
	}

}