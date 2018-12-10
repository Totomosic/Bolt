#include "Types.h"
#include "SGQTransparency.h"
#include "SGQComponents.h"
#include "..\ObjectCollection.h"
#include "..\Components\MeshRenderer.h"

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
			bool passed = false;
			for (int i = 0; i < Mesh::MAX_MATERIALS; i++)
			{
				if (RequiresTransparency)
				{
					if (meshRenderer.Mesh.Materials[i].HasTransparency())
					{
						passed = true;
						break;
					}
				}
				else
				{
					if (meshRenderer.Mesh.Materials[i].HasTransparency())
					{
						passed = false;
						break;
					}
					else
					{
						passed = true;
					}
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