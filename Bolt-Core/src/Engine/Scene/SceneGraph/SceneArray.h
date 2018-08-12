#pragma once
#include "SceneGraph.h"

namespace Bolt
{

	struct BLT_API SceneArray : public SceneGraph
	{
	public:
		static constexpr int MAX_GAMEOBJECTS = 10000;

	private:
		GameObject* m_GameObjects[MAX_GAMEOBJECTS];
		mutable int m_MaxGameObjectID;

	public:
		SceneArray();

		id_t AddGameObject(GameObject* gameObject) override;

		const GameObject& GetGameObjectById(id_t id) const override;
		GameObject& GetGameObjectById(id_t id) override;

		std::vector<GameObject*> GetAllGameObjects() const override;

		SGQueryResult Query(const SGQuery& query) const override;

	};

}