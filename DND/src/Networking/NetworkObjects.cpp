#include "bltpch.h"
#include "NetworkObjects.h"

#include "NetworkIdentity.h"

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

	GameObject* NetworkObjects::GetObjectByNetworkId(id_t networkId) const
	{
		return m_NetworkObjects.at(networkId);
	}

	void NetworkObjects::IdentifyObject(GameObject* object)
	{
		IdentifyObject(GetNextNetworkId(), object);
	}

	void NetworkObjects::IdentifyObject(id_t networkId, GameObject* object)
	{
		m_NetworkObjects[networkId] = object;
		object->Components().AddComponent<NetworkIdentity>(networkId);
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

}