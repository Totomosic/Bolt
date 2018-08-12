#pragma once
#include "SGQuery.h"

namespace Bolt
{

	struct BLT_API SGQFrustrum : public SGQuery
	{
	public:
		Frustum Frustrum;
	};

}