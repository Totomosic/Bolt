#pragma once
#include "ScenePackets.h"
#include "Players/PlayerManager.h"
#include "DndServer.h"

namespace DND
{

	class DndInstance : public Component
	{
	private:
		TilemapManager* m_MapManager;
		NetworkObjectManager m_Objects;
		PlayerManager m_Players;
		DndServer m_Server;

		GameObject* m_TileSelector;

	public:
		DndInstance(TilemapManager& mapManager, ObjectFactory& entityFactory);

		TilemapManager& MapManager() const;

		void StartGame(EntityNetworkData myCharacter);
		void StartGame(const AddressPair& address, EntityNetworkData myCharacter, std::function<void()> timeoutCallback);
		void Update() override;
		void End() override;

		WelcomePacket GetWelcomePacket() const;

		std::unique_ptr<Component> Clone() const override;

	private:
		void StartGameFromWelcome(const WelcomePacket& welcome, EntityNetworkData myCharacter);
		void ConnectToHost(const AddressPair& host, std::function<void(WelcomePacket)> callback, std::function<void()> timeoutCallback);

	};

}