#pragma once
#include "../MeshData.h"

namespace Bolt
{

	struct BLT_API VertexFactory
	{
	public:
		virtual MeshData GenerateVertices() const = 0;

	};

}