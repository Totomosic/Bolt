#include "SGQComponents.h"
#include "..\ObjectCollection.h"

namespace Bolt
{

	SGQComponents::SGQComponents(const std::vector<std::type_index>& componentTypes) : SGQuery(),
		ComponentTypes(componentTypes)
	{
	
	}

	std::vector<GameObject*> SGQComponents::Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const
	{
		std::vector<GameObject*> result;
		std::vector<GameObject*> objects = graph.GetAllGameObjects();
		for (GameObject* object : objects)
		{
			bool hasComponents = true;
			for (const std::type_index& type : ComponentTypes)
			{
				if (!object->Components().HasComponent(type.hash_code()))
				{
					hasComponents = false;
					break;
				}
			}
			if (hasComponents)
			{
				result.push_back(object);
				*mostRelevant = object;
				*leastRelevant = object;
			}
		}
		return result;
	}

}