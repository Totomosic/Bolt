#pragma once
#include "bltpch.h"

namespace DND
{

	class NetworkObjects
	{
	private:
		IdManager<id_t> m_NetworkIdManager;
		std::unordered_map<id_t, GameObject*> m_NetworkObjects;

	public:
		NetworkObjects();

		id_t GetNextNetworkId() const;
		id_t PeekNextNetworkId() const;
		void SetNextAvailableNetworkId(id_t id);

		GameObject* GetObjectByNetworkId(id_t networkId) const;
		void IdentifyObject(GameObject* object);
		void IdentifyObject(id_t networkId, GameObject* object);
		void ReleaseObject(id_t networkId);
		void Clear();

	};

}