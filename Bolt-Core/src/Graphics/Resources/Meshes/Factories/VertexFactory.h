#pragma once
#include "..\..\..\Buffers\__Buffers__.h"

namespace Bolt
{

	struct BLT_API ModelBounds
	{
	public:
		float MinX;
		float MaxX;
		float MinY;
		float MaxY;
		float MinZ;
		float MaxZ;
	};

	struct BLT_API ModelData
	{
	public:
		std::unique_ptr<VertexArray> Vertices;
		std::unique_ptr<IndexArray> Indices;
		ModelBounds Bounds;
	};

	struct BLT_API VertexFactory
	{
	public:
		virtual ModelData GenerateVertices() const = 0;

	};

}