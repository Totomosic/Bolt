#include "bltpch.h"
#include "DndInstance.h"
#include "NetworkManager.h"

namespace DND
{

	DndInstance::DndInstance(DndInitData initData) : Component(),
		m_Tilemap(initData.MapManager), m_ObjectManager(initData.Factory, m_Tilemap), m_PlayerManager(m_ObjectManager), m_Camera(initData.MainCamera)
	{
		NetworkManager::Get().RegisterAsHost();
		m_Tilemap.LoadTilemapAsync(0, [this, callback = std::move(initData.TilemapReadyCallback)]()
		{
			m_Tilemap.SetCurrentMap(0);
			callback();
		});		
	}

	void DndInstance::End()
	{
		NetworkManager::Get().RemoveAsHost();
		m_Tilemap.UnloadAllTilemaps();
	}

	void DndInstance::CreateGame(const WelcomePacket& data)
	{
		
	}

	std::unique_ptr<Component> DndInstance::Clone() const
	{
		BLT_ASSERT(false, "Cannot clone Dnd Instance");
		return std::unique_ptr<DndInstance>();
	}

}