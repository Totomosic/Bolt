#pragma once
#include "SGQuery.h"
#include "..\..\GameObject.h"

namespace Bolt
{

	struct BLT_API SGQueryResult
	{
	public:
		std::vector<GameObject*> GameObjects;
		GameObject* MostRelevant = nullptr;
		GameObject* LeastRelevant = nullptr;

	};

}