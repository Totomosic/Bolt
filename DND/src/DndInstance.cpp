#include "bltpch.h"
#include "DndInstance.h"
#include "NetworkManager.h"

namespace DND
{

	DndInstance::DndInstance(TilemapManager&& tilemap, ObjectFactory&& factory, Camera* mainCamera) : Component(),
		m_Tilemap(std::move(tilemap)), m_ObjectManager(std::move(factory), m_Tilemap), m_PlayerManager(m_ObjectManager), m_Camera(mainCamera)
	{
		
	}

	void DndInstance::Start()
	{
		NetworkManager::Get().RegisterAsHost();
		m_Tilemap.LoadTilemap(0);
		m_Tilemap.SetCurrentMap(0);

	}

	void DndInstance::End()
	{
		NetworkManager::Get().RemoveAsHost();
	}

	std::unique_ptr<Component> DndInstance::Clone() const
	{
		BLT_ASSERT(false, "Cannot clone Dnd Instance");
		return std::unique_ptr<DndInstance>();
	}

}