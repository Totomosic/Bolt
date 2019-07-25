#include "bltpch.h"
#include "DndInstance.h"

#include "GameplayPackets.h"
#include "NetworkManager.h"
#include "Players/Camera/CameraController.h"
#include "Players/Characters/Entities/TileMotion.h"

namespace DND
{

	DndInstance::DndInstance(TilemapManager& mapManager, ObjectFactory& entityFactory) : Component(),
		m_MapManager(&mapManager), m_Objects(entityFactory, MapManager()), m_Players(m_Objects), m_Server(m_Players), m_TileSelector(nullptr)
	{
		m_Objects.m_Server = &m_Server;

		NetworkManager::Get().Server().AddPacketListener(PacketType::Hello, [this](ReceivedPacket& p)
		{
			BLT_INFO("RECEIVED HELLO PACKET FROM {}", p.FromAddress);
			WelcomePacket packet = GetWelcomePacket();
			NetworkManager::Get().SendPacket(p.FromAddress, packet);
			return false;
		});

		NetworkManager::Get().Server().AddPacketListener(PacketType::Introduction, [this](ReceivedPacket& p)
		{
			BLT_INFO("RECEIVED INTRODUCTION PACKET FROM {}", p.FromAddress);
			IntroductionPacket packet;
			Deserialize(p.Packet, packet);
			
			id_t networkId = m_Objects.NetworkIds().GetNextId();
			id_t playerId = m_Players.PlayerIds().GetNextId();
			BLT_ASSERT(packet.PlayerId == playerId, "PlayerId Mismatch");
			BLT_ASSERT(packet.PlayerObject.NetworkId == networkId, "NetworkId Mismatch");

			PlayerNetworkData player;
			player.Address = packet.Address;
			player.PlayerId = packet.PlayerId;
			player.PlayerObject = packet.PlayerObject;
			m_Players.AddPlayer(player);

			return false;
		});

		NetworkManager::Get().Server().AddPacketListener(PacketType::ConnectToHost, [this](ReceivedPacket& p)
		{
			ConnectToHostPacket connectPacket;
			Deserialize(p.Packet, connectPacket);
			ConnectionEstablishedPacket packet;
			NetworkManager::Get().SendPacket(connectPacket.HostAddress.PrivateEndpoint, packet);
			BLT_INFO("SENT CONNECTION ESTABLISHED PACKET TO {}", connectPacket.HostAddress.PrivateEndpoint);
			return false;
		});

		NetworkManager::Get().Server().AddPacketListener(PacketType::Disconnect, [this](ReceivedPacket& p)
		{
			BLT_INFO("RECEIVED DISCONNECT PACKET FROM {}", p.FromAddress);
			DisconnectPacket packet;
			Deserialize(p.Packet, packet);
			BLT_INFO("REMOVING PLAYER WITH ID {}", packet.PlayerId);
			if (m_Players.HasPlayerId(packet.PlayerId))
			{
				m_Players.RemovePlayer(packet.PlayerId);
			}
			return false;
		});

		NetworkManager::Get().Server().AddPacketListener(PacketType::MoveToTile, [this](ReceivedPacket& p)
		{
			MoveToTilePacket packet;
			Deserialize(p.Packet, packet);
			GameObject* object = m_Objects.GetObjectByNetworkId(packet.NetworkId);
			object->Components().GetComponent<TileMotion>().MoveToBottomLeftTile(packet.MoveTile);
			return false;
		});
	}

	TilemapManager& DndInstance::MapManager() const
	{
		return *m_MapManager;
	}

	void DndInstance::StartGame(EntityNetworkData myCharacter)
	{
		WelcomePacket packet;
		packet.NetworkId = m_Objects.NetworkIds().GetNextId();
		packet.PlayerId = m_Players.PlayerIds().GetNextId();
		StartGameFromWelcome(packet, std::move(myCharacter));
	}

	void DndInstance::StartGame(const AddressPair& address, EntityNetworkData myCharacter, std::function<void()> timeoutCallback)
	{
		ConnectToHost(address, [this, myCharacter](WelcomePacket packet)
		{
			StartGameFromWelcome(packet, myCharacter);
		}, std::move(timeoutCallback));
	}

	void DndInstance::Update()
	{
		if (m_TileSelector != nullptr)
		{
			Camera& c = *camera();
			Vector3f mousePos = Input::Get().MousePosition(c.ViewWidth(), c.ViewHeight());
			Vector2f worldPos = mousePos.xy() + c.transform().Position().xy();
			Tile selectedTile = m_MapManager->CurrentMap().TileFromWorldPosition(worldPos);
			if (selectedTile.x >= 0 && selectedTile.y >= 0)
			{
				Vector3f tilePos = m_MapManager->CurrentMap().WorldPositionOfTile(selectedTile);
				m_TileSelector->transform().SetLocalXY(tilePos.xy());
			}
		}
	}

	void DndInstance::End()
	{
		DisconnectPacket packet;
		packet.PlayerId = m_Players.LocalPlayer().PlayerId();
		m_Server.SendPacketToAll(packet);
		NetworkManager::Get().RemoveAsHost();
		MapManager().UnloadAllTilemaps();

		m_Objects.NetworkIds().Reset();
		m_Players.PlayerIds().Reset();
		NetworkManager::Get().Server().ClearPacketListeners();
	}

	WelcomePacket DndInstance::GetWelcomePacket() const
	{
		WelcomePacket result;
		result.PlayerId = m_Players.PlayerIds().PeekNextId();
		result.NetworkId = m_Objects.NetworkIds().PeekNextId();
		result.GameState.Players = m_Players.GetAllPlayerData();
		return result;
	}

	std::unique_ptr<Component> DndInstance::Clone() const
	{
		BLT_ASSERT(false, "Cannot Clone DndInstance");
		return std::unique_ptr<DndInstance>();
	}

	void DndInstance::ConnectToHost(const AddressPair& address, std::function<void(WelcomePacket)> callback, std::function<void()> timeoutCallback)
	{
		SocketAddress addr = address.PrivateEndpoint;
		BLT_INFO("ATTEMPTING TO CONNECT TO {}", addr);
		NetworkManager::Get().Server().AddPacketListenerTimeout(PacketType::ConnectionEstablished, 5.0f, [addr, callback, timeoutCallback](ReceivedPacket& p)
		{
			BLT_INFO("RECEIVED CONNECTION ESTABLISHED PACKET FROM {}", p.FromAddress);
			if (addr == p.FromAddress)
			{				
				NetworkManager::Get().Server().AddPacketListenerTimeout(PacketType::Welcome, 5.0f, [addr, callback{ std::move(callback) }](ReceivedPacket& p)
				{
					BLT_INFO("RECEVIED WELCOME PACKET FROM {}", p.FromAddress);
					if (addr == p.FromAddress)
					{						
						WelcomePacket packet;
						Deserialize(p.Packet, packet);
						callback(packet);
					}
					return addr == p.FromAddress;
				}, timeoutCallback);
				HelloPacket packet;
				NetworkManager::Get().SendPacket(p.FromAddress, packet);
			}
			return addr == p.FromAddress;
		}, timeoutCallback);

		ConnectToHostPacket packet;
		packet.HostAddress = address;
		packet.PrivateEndpoint = NetworkManager::Get().Address().PrivateEndpoint;
		m_Server.SendPacketToEc2(packet);
	}

	void DndInstance::StartGameFromWelcome(const WelcomePacket& packet, EntityNetworkData myCharacter)
	{
		NetworkManager::Get().RegisterAsHost();
		Camera& mainCamera = *camera();

		id_t currentMapId = myCharacter.MapId;
		MapManager().LoadTilemap(currentMapId);
		MapManager().SetCurrentMap(currentMapId);

		BLT_INFO("MY PLAYER ID {}", packet.PlayerId);
		myCharacter.NetworkId = packet.NetworkId;
		PlayerNetworkData myPlayer;
		myPlayer.Address = NetworkManager::Get().Address();
		myPlayer.PlayerId = packet.PlayerId;
		myPlayer.PlayerObject = myCharacter;
		m_Players.SetLocalPlayer(std::move(myPlayer));

		m_Objects.NetworkIds().SetNextAvailableId(packet.NetworkId + 1);
		m_Players.PlayerIds().SetNextAvailableId(packet.PlayerId + 1);

		for (const PlayerNetworkData& otherPlayer : packet.GameState.Players)
		{
			m_Players.AddPlayer(otherPlayer);
		}

		IntroductionPacket introduction;
		introduction.Address = NetworkManager::Get().Address();
		introduction.PlayerId = packet.PlayerId;
		introduction.PlayerObject = myCharacter;
		m_Server.SendPacketToAll(introduction);

		mainCamera.Components().AddComponent<CameraController>(m_Players.LocalPlayer().Object());
		m_TileSelector = m_Objects.Factory().Rectangle(m_MapManager->TileWidth(), m_MapManager->TileHeight(), Color(200, 0, 0, 150));
	}

}