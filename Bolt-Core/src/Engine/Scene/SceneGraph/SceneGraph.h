#pragma once
#include "Bolt-Graphics.h"
#include "..\GameObject.h"
#include "Query\__Query__.h"
#include "..\ObjectCollection.h"

namespace Bolt
{

	struct BLT_API SceneGraph
	{
	protected:
		const ObjectCollection* m_ObjectCollection;

	public:
		SceneGraph();

		virtual id_t AddGameObject(GameObject* gameObject) = 0;

		virtual const GameObject& GetGameObjectById(id_t id) const = 0;
		virtual GameObject& GetGameObjectById(id_t id) = 0;

		virtual std::vector<GameObject*> GetAllGameObjects() const = 0;

		virtual SGQueryResult Query(const SGQuery& query) const = 0;

	public:
		virtual void SetObjectCollection(const ObjectCollection* objects);

	};

}