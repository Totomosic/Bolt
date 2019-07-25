#pragma once
#include "SGQuery.h"

namespace Bolt
{

	class BLT_API SGQTransparency : public SGQuery
	{
	public:
		bool RequiresTransparency;

	public:
		SGQTransparency(bool requiresTransparency = true);

		std::vector<GameObject*> Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const override;

	};


}