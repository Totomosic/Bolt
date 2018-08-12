#pragma once
#include "Bolt-Graphics.h"
#include "..\GameObject.h"
#include "Query\__Query__.h"

namespace Bolt
{

	struct BLT_API SceneGraph
	{
	public:
		virtual id_t AddGameObject(GameObject* gameObject) = 0;

		virtual const GameObject& GetGameObjectById(id_t id) const = 0;
		virtual GameObject& GetGameObjectById(id_t id) = 0;

		virtual std::vector<GameObject*> GetAllGameObjects() const = 0;

		virtual SGQueryResult Query(const SGQuery& query) const = 0;

	};

}