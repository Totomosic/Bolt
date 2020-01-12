#pragma once
#include "../Vectors/Vector3.h"

namespace Bolt
{

	struct BLT_API Cuboid
	{
	public:
		Vector3f Min;
		Vector3f Max;

	public:
		inline float Width() const { return Max.x - Min.x; }
		inline float Height() const { return Max.y - Min.y; }
		inline float Depth() const { return Max.z - Min.z; }
		inline Vector3f Center() const { return Min + Vector3f(Width() / 2.0f, Height() / 2.0f, Depth() / 2.0f); }

	};

}