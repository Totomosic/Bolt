#include "SGQComponents.h"
#include "..\SceneGraph.h"

namespace Bolt
{

	SGQComponents::SGQComponents(const std::vector<std::type_index>& componentTypes) : SGQuery(),
		ComponentTypes(componentTypes)
	{
	
	}

	std::vector<GameObject*> SGQComponents::Evaluate(const SceneGraph& graph, GameObject** mostRelevant, GameObject** leastRelevant) const
	{
		std::vector<GameObject*> result;
		std::vector<GameObject*> objects = graph.GetAllGameObjects();
		for (GameObject* object : objects)
		{
			bool hasComponents = true;
			for (const std::type_index& type : ComponentTypes)
			{
				if (!object->Components().HasComponent(type))
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