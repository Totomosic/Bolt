#pragma once
#include "..\..\..\Buffers\__Buffers__.h"

namespace Bolt
{

	struct BLT_API ModelData
	{
	public:
		std::unique_ptr<VertexArray> Vertices;
		IndexArray Indices;
	};

	struct BLT_API VertexFactory
	{
	public:
		virtual ModelData GenerateVertices() const = 0;

	};

}