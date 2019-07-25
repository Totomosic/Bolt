#include "bltpch.h"

#include "SGQTransparency.h"
#include "SGQComponents.h"
#include "../ObjectCollection.h"
#include "../Components/MeshRenderer.h"

namespace Bolt
{

	SGQTransparency::SGQTransparency(bool requiresTransparency) : SGQuery(),
		RequiresTransparency(requiresTransparency)
	{
	
	}

	std::vector<GameObject*> SGQTransparency::Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const
	{
		std::vector<GameObject*> result;
		std::vector<GameObject*> objects = graph.Query(SGQComponents({ typeid(MeshRenderer) })).GameObjects;
		for (GameObject* object : objects)
		{
			MeshRenderer& meshRenderer = object->Components().GetComponent<MeshRenderer>();
			bool passed = !RequiresTransparency;
			for (const auto& material : meshRenderer.Mesh.Materials)
			{
				if (RequiresTransparency && material->IsTransparent())
				{
					passed = true;
					break;
				}
				else if (!RequiresTransparency && material->IsTransparent())
				{
					passed = false;
					break;
				}
			}
			if (passed)
			{
				result.push_back(object);
				*mostRelevant = object;
				*leastRelevant = object;
			}
		}
		return result;
	}

}