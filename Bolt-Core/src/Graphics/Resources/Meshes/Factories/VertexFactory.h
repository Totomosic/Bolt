#pragma once
#include "..\..\..\Buffers\__Buffers__.h"

namespace Bolt
{

	struct BLT_API ModelBounds
	{
	public:
		float MinX = 0;
		float MaxX = 0;
		float MinY = 0;
		float MaxY = 0;
		float MinZ = 0;
		float MaxZ = 0;
	};

	struct BLT_API ModelData
	{
	public:
		std::unique_ptr<VertexArray> Vertices;
		IndexArray Indices;
		ModelBounds Bounds;
	};

	struct BLT_API VertexFactory
	{
	public:
		virtual ModelData GenerateVertices() const = 0;

	};

}