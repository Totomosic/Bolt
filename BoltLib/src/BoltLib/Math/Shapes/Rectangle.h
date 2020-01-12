#pragma once
#include "../Vectors/Vector2.h"

namespace Bolt
{

	struct BLT_API Rectangle
	{
	public:
		Vector2f Min;
		Vector2f Max;

	public:
		inline float Width() const { return Max.x - Min.x; }
		inline float Height() const { return Max.y - Min.y; }
		inline Vector2f Center() const { return Min + Vector2f(Width() / 2.0f, Height() / 2.0f); }

	};

}