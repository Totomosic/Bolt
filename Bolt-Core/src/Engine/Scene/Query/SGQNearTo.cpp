#include "Types.h"

#include "SGQNearTo.h"
#include "../ObjectCollection.h"

namespace Bolt
{

	SGQNearTo::SGQNearTo()
	{
		QType = QueryType::NearTo;
	}

	std::vector<GameObject*> SGQNearTo::Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const
	{
		std::vector<GameObject*> result;
		std::vector<GameObject*> objects = graph.GetAllGameObjects();
		float minDistance = +FLT_MAX;
		float maxDistance = -FLT_MAX;
		int objectCount = 0;

		for (GameObject* object : objects)
		{
			if (object->Id() != GameObject::InvalidID)
			{
				Vector3f toGameObject = object->transform().Position() - Point;
				float length = toGameObject.Length();
				if (length < MinDistance)
				{
					continue;
				}
				if (length > MaxDistance && MaxDistance != -1.0f)
				{
					continue;
				}
				result.push_back(object);
				objectCount++;
				if (length > maxDistance)
				{
					*leastRelevant = object;
					maxDistance = length;
				}
				if (length < minDistance)
				{
					*mostRelevant = object;
					minDistance = length;
				}
			}
		}

		return result;
	}

}