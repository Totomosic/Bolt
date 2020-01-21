#pragma once
#include "Vectors/Vector4.h"

namespace Bolt
{

	/*struct BLT_API Plane
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

	};*/

	using Plane = Vector4f;

}