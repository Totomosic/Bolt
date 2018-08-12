#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	struct BLT_API GeometryData
	{
	public:
		const VertexArray* Vertices = nullptr;
		const IndexBuffer* Indices = nullptr;
		Matrix4f Transform = Matrix4f::Identity();
		uint IndexOverride = (uint)-1;
	};

	struct BLT_API RenderBatch
	{
	public:
		const Material* Material;
		std::vector<GeometryData> Geometry;
	};

}