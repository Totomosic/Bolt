#include "bltpch.h"
#include "NetworkObjectManager.h"

namespace DND
{

	NetworkObjectManager::NetworkObjectManager(ObjectFactory& factory, TilemapManager& mapManager)
		: m_NetworkIdManager(0, GameObject::InvalidID - 1), m_Objects(), m_Factory(&factory), m_MapManager(&mapManager)
	{
	
	}

	const IdManager<id_t>& NetworkObjectManager::NetworkIds() const
	{
		return m_NetworkIdManager;
	}

	IdManager<id_t>& NetworkObjectManager::NetworkIds()
	{
		return m_NetworkIdManager;
	}

	const ObjectFactory& NetworkObjectManager::Factory() const
	{
		return *m_Factory;
	}

	ObjectFactory& NetworkObjectManager::Factory()
	{
		return *m_Factory;
	}

	bool NetworkObjectManager::HasObjectWithNetworkId(id_t networkId) const
	{
		return m_Objects.find(networkId) != m_Objects.end();
	}

	const NetworkObjectManager::NetworkObject& NetworkObjectManager::GetObjectInfoByNetworkId(id_t networkId) const
	{
		return m_Objects.at(networkId);
	}

	GameObject* NetworkObjectManager::GetObjectByNetworkId(id_t networkId) const
	{
		return GetObjectInfoByNetworkId(networkId).Object;
	}

	void NetworkObjectManager::AddObject(id_t networkId, GameObject* object, id_t ownerNetworkId)
	{
		m_Objects[networkId] = { object, GameObject::InvalidID, {} };
		object->Components().AddComponent<NetworkIdentity>(networkId);
		if (ownerNetworkId != GameObject::InvalidID)
		{
			SetObjectParent(networkId, ownerNetworkId);
		}
	}

	void NetworkObjectManager::RemoveObject(id_t networkId)
	{
		NetworkObject& object = m_Objects.at(networkId);
		if (object.ParentNetworkId != GameObject::InvalidID)
		{
			NetworkObject& parent = m_Objects.at(object.ParentNetworkId);
			auto it = std::find(parent.OwnedNetworkIds.begin(), parent.OwnedNetworkIds.end(), networkId);
			parent.OwnedNetworkIds.erase(it);
		}
		for (int i = object.OwnedNetworkIds.size() - 1; i >= 0; i--)
		{
			id_t childId = object.OwnedNetworkIds[i];
			RemoveObject(networkId);
		}
		Destroy(object.Object);
		m_Objects.erase(networkId);
	}

	void NetworkObjectManager::RemoveObject(GameObject* object)
	{
		id_t networkId = object->Components().GetComponent<NetworkIdentity>().NetworkId;
		RemoveObject(networkId);
	}

	void NetworkObjectManager::ClearObjects()
	{
		for (auto& pair : m_Objects)
		{
			Destroy(pair.second.Object);
		}
		m_Objects.clear();
	}

	GameObject* NetworkObjectManager::CreateFromNetworkData(const EntityNetworkData& data, id_t ownerNetworkId)
	{
		GameObject* object = Factory().Instantiate(Factory().GetPrefab(data.PrefabId));
		object->Components().AddComponent<CurrentMap>(data.MapId, m_MapManager);
		AddObject(data.NetworkId, object, ownerNetworkId);
		for (const EntityNetworkData& childData : data.OwnedObjects)
		{
			CreateFromNetworkData(childData, data.NetworkId);
		}
		return object;
	}

	EntityNetworkData NetworkObjectManager::GetEntityData(id_t networkId) const
	{
		const NetworkObject& object = GetObjectInfoByNetworkId(networkId);
		EntityNetworkData data;
		data.NetworkId = networkId;
		data.MapId = object.Object->Components().GetComponent<CurrentMap>().MapId();
		for (id_t childNetId : object.OwnedNetworkIds)
		{
			data.OwnedObjects.push_back(GetEntityData(childNetId));
		}
		return data;
	}

	EntityNetworkData NetworkObjectManager::GetEntityData(GameObject* object) const
	{
		id_t networkId = object->Components().GetComponent<NetworkIdentity>().NetworkId;
		return GetEntityData(networkId);
	}

	void NetworkObjectManager::SetObjectParent(id_t networkId, id_t parentNetworkId)
	{
		NetworkObject& object = m_Objects.at(networkId);
		NetworkObject& parent = m_Objects.at(parentNetworkId);
		object.ParentNetworkId = parentNetworkId;
		parent.OwnedNetworkIds.push_back(networkId);
	}

}