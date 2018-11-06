#pragma once
#include "Bolt-Core.h"
#include "..\..\GameObject.h"

namespace Bolt
{

	struct SceneGraph;

	BLT_API enum class QueryType
	{
		None,
		NearTo,
		Frustum
	};

	// Represents a Scene Graph Query
	struct BLT_API SGQuery
	{
	public:
		QueryType QType;

	public:
		SGQuery();

		virtual std::vector<GameObject*> Evaluate(const SceneGraph& graph, GameObject** mostRelevant, GameObject** leastRelevant) const = 0;

	};

}