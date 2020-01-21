#pragma once
#include "Component.h"
#include "BoltLib/Math/Vectors/Vector2.h"

namespace Bolt
{

	struct BLT_API BoundingRectangle : public Component<BoundingRectangle>
	{
	public:
		Vector2f Min;
		Vector2f Max;
	};

}