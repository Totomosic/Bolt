#pragma once
#include "Vectors\__Vectors__.h"

namespace Bolt
{

	struct BLT_API Plane
	{
	public:
		union
		{
			struct
			{
				Vector3f Normal;
				float Distance;
			};
			Vector4f NormDist;
		};

	};

}