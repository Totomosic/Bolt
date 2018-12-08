#pragma once
#include "..\GameObject.h"

namespace Bolt
{

	struct ObjectCollection;

	BLT_API enum class QueryType
	{
		None,
		NearTo,
		Frustum
	};

	// Represents a Scene Graph Query
	class BLT_API SGQuery
	{
	public:
		QueryType QType;

	public:
		SGQuery();

		virtual std::vector<GameObject*> Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const = 0;

	};

}