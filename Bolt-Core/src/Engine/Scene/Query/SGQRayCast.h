#pragma once
#include "SGQuery.h"

namespace Bolt
{

	// Represents a ray cast test, result contains a sorted list of hit GameObjects in order of distance from ray origin
	class BLT_API SGQRayCast : public SGQuery
	{
	public:
		Ray ray;

	public:
		SGQRayCast(Ray ray);

		std::vector<GameObject*> Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const override;

	};

}