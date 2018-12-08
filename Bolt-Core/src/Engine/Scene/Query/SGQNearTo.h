#pragma once
#include "SGQuery.h"

namespace Bolt
{

	class BLT_API SGQNearTo : public SGQuery
	{
	public:
		Vector3f Point = Vector3f(0.0f, 0.0f, 0.0f);
		float MinDistance = 0;
		float MaxDistance = -1;

	public:
		SGQNearTo();

		std::vector<GameObject*> Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const override;

	};

}