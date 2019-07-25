#pragma once
#include "../ModelData.h"

namespace Bolt
{

	struct BLT_API VertexFactory
	{
	public:
		virtual ModelData GenerateVertices() const = 0;

	};

}