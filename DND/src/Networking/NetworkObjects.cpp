#include "bltpch.h"
#include "NetworkObjects.h"

#include "NetworkIdentity.h"
#include "../Entities/Characters/CharacterPrefabId.h"
#include "../Entities/Characters/CharacterStats.h"
#include "../Entities/TileTransform.h"

namespace DND
{

	NetworkObjects::NetworkObjects()
		: m_NetworkIdManager(0, GameObject::InvalidID - 1), m_NetworkObjects()
	{
	
	}

	id_t NetworkObjects::GetNextNetworkId() const
	{
		return m_NetworkIdManager.GetNextId();
	}

	id_t NetworkObjects::PeekNextNetworkId() const
	{
		return m_NetworkIdManager.PeekNextId();
	}

	void NetworkObjects::SetNextAvailableNetworkId(id_t networkId)
	{
		m_NetworkIdManager.SetNextAvailableId(networkId);
	}

	bool NetworkObjects::HasObject(id_t networkId) const
	{
		return m_NetworkObjects.find(networkId) != m_NetworkObjects.end();
	}

	const NetworkObjects::ObjectInfo& NetworkObjects::GetObjectInfoByNetworkId(id_t networkId) const
	{
		return m_NetworkObjects.at(networkId);
	}

	GameObject* NetworkObjects::GetObjectByNetworkId(id_t networkId) const
	{
		return m_NetworkObjects.at(networkId).Object;
	}

	void NetworkObjects::IdentifyObject(GameObject* object, id_t owner)
	{
		IdentifyObject(GetNextNetworkId(), object);
	}

	void NetworkObjects::IdentifyObject(id_t networkId, GameObject* object, id_t owner)
	{
		m_NetworkObjects[networkId] = { GameObject::InvalidID, object, {} };
		object->Components().AddComponent<NetworkIdentity>(networkId);
		if (owner != GameObject::InvalidID)
		{
			SetOwner(networkId, owner);
		}
	}

	void NetworkObjects::SetOwner(id_t object, id_t owner)
	{
		ObjectInfo& objectInfo = m_NetworkObjects.at(object);
		ObjectInfo& ownerInfo = m_NetworkObjects.at(owner);
		BLT_ASSERT(objectInfo.Owner == GameObject::InvalidID, "Already has owner");
		ownerInfo.OwnedObjects.push_back(object);
		objectInfo.Owner = owner;
	}

	void NetworkObjects::DestroyObject(id_t networkId)
	{
		ObjectInfo& info = m_NetworkObjects.at(networkId);
		if (info.Owner != GameObject::InvalidID)
		{
			ObjectInfo& owner = m_NetworkObjects.at(info.Owner);
			auto it = std::find(owner.OwnedObjects.begin(), owner.OwnedObjects.end(), networkId);
			if (it != owner.OwnedObjects.end())
			{
				owner.OwnedObjects.erase(it);
			}
		}
		Destroy(info.Object);
		info.Object = nullptr;
		info.Owner = GameObject::InvalidID;
		for (int i = info.OwnedObjects.size() - 1; i >= 0; i--)
		{
			id_t object = info.OwnedObjects[i];
			DestroyObject(object);
		}
		ReleaseObject(networkId);
	}

	void NetworkObjects::ReleaseObject(id_t networkId)
	{
		m_NetworkObjects.erase(networkId);
		m_NetworkIdManager.ReleaseId(networkId);
	}

	void NetworkObjects::Clear()
	{
		m_NetworkObjects.clear();
		m_NetworkIdManager.Reset();
	}

	CharacterInfo NetworkObjects::GetCharacterInfo(id_t networkId) const
	{
		const ObjectInfo& objInfo = GetObjectInfoByNetworkId(networkId);
		CharacterInfo character;
		character.NetworkId = networkId;
		character.PrefabId = objInfo.Object->Components().GetComponent<CharacterPrefabId>().PrefabId;
		character.MapId = objInfo.Object->Components().GetComponent<TileTransform>().CurrentMapId();
		character.CurrentTile = objInfo.Object->Components().GetComponent<TileTransform>().CurrentTile();
		character.Stats = objInfo.Object->Components().GetComponent<StatsComponent>().Stats();
		for (id_t ownedObject : objInfo.OwnedObjects)
		{
			character.OwnedCharacters.push_back(GetCharacterInfo(ownedObject));
		}
		return character;
	}

}