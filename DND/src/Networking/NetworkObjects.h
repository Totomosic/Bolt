#pragma once
#include "bltpch.h"
#include "GamePlayPacketSerialization.h"

namespace DND
{

	class NetworkObjects
	{
	public:
		struct ObjectInfo
		{
		public:
			id_t Owner = GameObject::InvalidID;
			GameObject* Object = nullptr;
			std::vector<id_t> OwnedObjects;
		};

	private:
		IdManager<id_t> m_NetworkIdManager;
		std::unordered_map<id_t, ObjectInfo> m_NetworkObjects;

	public:
		NetworkObjects();

		id_t GetNextNetworkId() const;
		id_t PeekNextNetworkId() const;
		void SetNextAvailableNetworkId(id_t id);

		bool HasObject(id_t networkId) const;
		const ObjectInfo& GetObjectInfoByNetworkId(id_t networkId) const;
		GameObject* GetObjectByNetworkId(id_t networkId) const;
		void IdentifyObject(GameObject* object, id_t ownerNetworkId = GameObject::InvalidID);
		void IdentifyObject(id_t networkId, GameObject* object, id_t ownerNetworkId = GameObject::InvalidID);
		void SetOwner(id_t object, id_t owner);
		void DestroyObject(id_t networkId);
		void ReleaseObject(id_t networkId);
		void Clear();

		CharacterInfo GetCharacterInfo(id_t networkId) const;

	};

}