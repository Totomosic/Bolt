#include "bltpch.h"
#include "ServerScene.h"
#include "../App.h"
#include "../GameManager.h"
#include "GameScene.h"

namespace DND
{

	void CreateServerScene(DndClient& client, Scene& serverScene, const ResourcePack& resources, Scene& gameScene)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* serverCamera = serverScene.CreateCamera(orthoProj);
		Layer& serverLayer = serverScene.CreateLayer(serverCamera);
		Layer& joinableGamesLayer = serverScene.CreateLayer(serverCamera);

		serverCamera->transform().Translate(-serverCamera->ViewWidth() / 2, -serverCamera->ViewHeight() / 2, 50);

		GameManager::Get().Network().Server().OnGetHostResponsePacket.Subscribe([&client, &joinableGamesLayer](id_t listenerId, ReceivedPacketEvent& e)
		{
			GetHostsResponsePacket packet;
			Deserialize(e.Packet, packet);
			float yOffset = 200;
			for (int i = 0; i < packet.HostPublicAddresses.size(); i++)
			{
				SocketAddress& hostPublicAddress = packet.HostPublicAddresses[i];
				SocketAddress& hostPrivateAddress = packet.HostPrivateAddresses[i];
				UIsurface& button = joinableGamesLayer.UI().Rectangle(300, 50, Color::White, Transform({ 0, yOffset, 0 }));
				button.Text(hostPublicAddress.ToString(), Color::Black);
				button.EventHandler().OnClicked.Subscribe([&client, hostPublicAddress, hostPrivateAddress](id_t listenerId, UIClickedEvent& e)
				{
					ConnectToHostPacket connectingPacket;
					connectingPacket.HostPublicAddress = hostPublicAddress;
					connectingPacket.PrivateAddress = GameManager::Get().Network().Server().Address();
					GameManager::Get().Network().Server().SendPacket(SocketAddress(client.EC2_ADDRESS, client.EC2_PORT), connectingPacket);
					GameManager::Get().Holepunch(hostPublicAddress, hostPrivateAddress, [&client](SocketAddress address)
					{
						BLT_CORE_INFO("Successfully holepunched to connect to {}", address.ToString());
						BLT_INFO("SENDING HELLO PACKET TO {}", address.ToString());
						PlayerCharacterInfo player;
						player.PrefabId = GameManager::Get().Prefabs().BlueWizard;
						GameManager::Get().Join(address, player, [&client](const WelcomePacket& welcome, const PlayerCharacterInfo& player)
						{
							AddHostPacket packet;
							packet.PrivateAddress = GameManager::Get().Network().Server().Address();
							GameManager::Get().Network().Server().SendPacket(SocketAddress(client.EC2_ADDRESS, client.EC2_PORT), packet);
							CreateSceneFromWelcome(welcome, player);
						});
						return false;
					});
					return true;
				});
				yOffset -= 55;
			}
			return true;
		});

		UIsurface& hostButton = serverLayer.UI().Rectangle(300, 100, Color::Green, Transform({ 0, -200, 0 }));
		hostButton.Text("Host");
		hostButton.EventHandler().OnClicked.Subscribe([&client](id_t listenerId, UIClickedEvent& e)
		{
			AddHostPacket packet;
			packet.PrivateAddress = GameManager::Get().Network().Server().Address();
			GameManager::Get().Network().Server().SendPacket(SocketAddress(client.EC2_ADDRESS, client.EC2_PORT), packet);
			PlayerCharacterInfo player;
			player.PrefabId = GameManager::Get().Prefabs().Swordsman;
			GameManager::Get().Host(player, CreateSceneFromWelcome);
			return true;
		});

		serverScene.OnLoad.Subscribe([&client](id_t listenerId, SceneLoadedEvent& e)
		{
			GameManager::Get().Network().Server().SetAddress(SocketAddress(client.ADDRESS, client.PORT));
			GameManager::Get().Network().Server().Bind();
			GameManager::Get().Network().Initialize();
			GetHostsPacket packet;
			GameManager::Get().Network().Server().SendPacket(SocketAddress(client.EC2_ADDRESS, client.EC2_PORT), packet);
			return true;
		});

		serverScene.OnUnload.Subscribe([&joinableGamesLayer](id_t listenerId, SceneUnloadedEvent& e)
		{
			joinableGamesLayer.UI().Clear();
			return true;
		});

	}

}