#pragma once
#include "Map/TilemapManager.h"
#include "Players/Characters/NetworkObjectManager.h"
#include "Players/PlayerManager.h"

#include "ScenePackets.h"

namespace DND
{

	class DndInstance : public Component
	{
	private:
		TilemapManager m_Tilemap;
		NetworkObjectManager m_ObjectManager;
		PlayerManager m_PlayerManager;
		Camera* m_Camera;

	public:
		DndInstance(TilemapManager&& tilemap, ObjectFactory&& factory, Camera* mainCamera);

		void Start() override;
		void End() override;

		std::unique_ptr<Component> Clone() const override;

	};

}