#pragma once
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexArray.h"
#include "BoltLib/Math/Shapes/Cuboid.h"

namespace Bolt
{

	struct BLT_API MeshData
	{
	public:
		std::unique_ptr<VertexArray> Vertices;
		std::unique_ptr<IndexArray> Indices;
		Cuboid Bounds;
	};

}