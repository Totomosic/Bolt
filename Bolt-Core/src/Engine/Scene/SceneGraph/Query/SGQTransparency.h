#pragma once
#include "SGQuery.h"

namespace Bolt
{

	struct BLT_API SGQTransparency : public SGQuery
	{
	public:
		bool RequiresTransparency;

	public:
		SGQTransparency(bool requiresTransparency = true);

		std::vector<GameObject*> Evaluate(const SceneGraph& graph, GameObject** mostRelevant, GameObject** leastRelevant) const override;

	};


}