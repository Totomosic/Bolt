#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Query\__Query__.h"

#ifndef GAMEOBJECTS_PER_LAYER
#define GAMEOBJECTS_PER_LAYER 20000
#endif

namespace Bolt
{

	struct BLT_API ObjectCollection
	{
	public:
		static constexpr size_t MAX_GAMEOBJECTS = GAMEOBJECTS_PER_LAYER;

		struct BLT_API GameObjectInfo
		{
		public:
			GameObject Object;
			bool Enabled = false;

		public:
			void Transfer(XMLserializer& backend, bool isWriting);

		};

	private:
		IdManager<id_t> m_IdManager;

		GameObjectInfo m_GameObjects[MAX_GAMEOBJECTS];
		std::vector<GameObject*> m_ActiveGameObjects;
		mutable std::unordered_map<blt::string, std::vector<GameObject*>> m_Tags;

	public:
		ObjectCollection();

		const GameObject& GetGameObjectById(id_t id) const;
		GameObject& GetGameObjectById(id_t id);
		const std::vector<GameObject*>& GetAllGameObjects() const;
		GameObject& GetGameObjectByTag(const blt::string& tag, int index = 0) const;
		const std::vector<GameObject*>& GetGameObjectsByTag(const blt::string& tag) const;
		bool TagExists(const blt::string& tag) const;

		virtual id_t AddGameObject(GameObject&& object);
		virtual void RemoveGameObject(GameObject* object);
		void RemoveGameObject(id_t id);
		void RemoveAllWithTag(const blt::string& tag);

		void TagGameObject(const blt::string& tag, GameObject* object) const;
		void RemoveAllTags(GameObject* object) const;
		void RemoveTags(GameObject* object, size_t count = 1) const;
		void RemoveTag(GameObject* object, const blt::string& tag) const;

		virtual SGQueryResult Query(const SGQuery& query) const;

		virtual void Reset();

		virtual void Transfer(XMLserializer& backend, bool isWriting);

	private:
		id_t FindNextId() const;
		void ReleaseId(id_t id) const;

	};

}