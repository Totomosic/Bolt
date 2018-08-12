#pragma once
#include "SGQuery.h"

namespace Bolt
{

	struct BLT_API SGQComponents : public SGQuery
	{
	public:
		std::vector<std::type_index> ComponentTypes;

	public:
		SGQComponents(const std::vector<std::type_index>& componentTypes);

		std::vector<GameObject*> Evaluate(const SceneGraph& graph, GameObject** mostRelevant, GameObject** leastRelevant) const override;

	};

}