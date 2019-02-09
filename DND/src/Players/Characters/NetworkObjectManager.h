#pragma once
#include "Entities/EntityNetworkData.h"
#include "Entities/CurrentMap.h"
#include "Entities/TileTransform.h"
#include "Entities/NetworkIdentity.h"

namespace DND
{

	class DndServer;

	class NetworkObjectManager
	{
	public:
		struct NetworkObject
		{
		public:
			GameObject* Object;
			id_t ParentNetworkId;
			std::vector<id_t> OwnedNetworkIds;
		};

	private:
		IdManager<id_t> m_NetworkIdManager;
		std::unordered_map<id_t, NetworkObject> m_Objects;

		ObjectFactory* m_Factory;
		TilemapManager* m_MapManager;
		DndServer* m_Server;

	public:
		NetworkObjectManager(ObjectFactory& factory, TilemapManager& mapManager);

		const IdManager<id_t>& NetworkIds() const;
		IdManager<id_t>& NetworkIds();
		const ObjectFactory& Factory() const;
		ObjectFactory& Factory();
		bool HasObjectWithNetworkId(id_t networkId) const;
		const NetworkObject& GetObjectInfoByNetworkId(id_t networkId) const;
		GameObject* GetObjectByNetworkId(id_t networkId) const;

		void AddObject(id_t networkId, GameObject* object, id_t ownerNetworkId = GameObject::InvalidID);
		void RemoveObject(id_t networkId);
		void RemoveObject(GameObject* object);
		void ClearObjects();

		GameObject* CreateFromNetworkData(const EntityNetworkData& data, id_t ownerNetworkId = GameObject::InvalidID);
		EntityNetworkData GetEntityData(id_t networkId) const;
		EntityNetworkData GetEntityData(GameObject* object) const;

		friend class DndInstance;

	private:
		void SetObjectParent(id_t networkId, id_t parentNetworkId);

	};

}