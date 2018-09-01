#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace Bolt
{

	struct BLT_API ObjectCollection
	{
	public:
		static constexpr size_t MAX_GAMEOBJECTS = 10000;

		struct BLT_API GameObjectInfo
		{
		public:
			GameObject Object;
			bool Enabled = false;
		};

	private:
		GameObjectInfo m_GameObjects[MAX_GAMEOBJECTS];
		std::vector<GameObject*> m_ActiveGameObjects;

	public:
		ObjectCollection();

		const GameObject& GetGameObjectById(id_t id) const;
		GameObject& GetGameObjectById(id_t id);
		const std::vector<GameObject*>& GetAllGameObjects() const;

		id_t AddGameObject(GameObject&& object);
		void RemoveGameObject(GameObject* object);
		void RemoveGameObject(id_t id);

	private:
		id_t FindNextId() const;

	};

}