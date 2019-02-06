#pragma once
#include "Map/TilemapManager.h"
#include "Players/Characters/NetworkObjectManager.h"
#include "Players/PlayerManager.h"

#include "ScenePackets.h"

namespace DND
{

	struct DndInitData
	{
	public:
		TilemapManager& MapManager;
		ObjectFactory& Factory;
		Camera* MainCamera;
		std::function<void()> TilemapReadyCallback;

	};

	class DndInstance : public Component
	{
	private:
		TilemapManager& m_Tilemap;
		NetworkObjectManager m_ObjectManager;
		PlayerManager m_PlayerManager;
		Camera* m_Camera;

	public:
		DndInstance(DndInitData init);

		void End() override;
		void CreateGame(const WelcomePacket& data);

		std::unique_ptr<Component> Clone() const override;

	};

}