#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	struct BLT_API RenderData
	{
	public:
		const VertexArray* Vertices = nullptr;
		const IndexBuffer* Indices = nullptr;
		Matrix4f Transform = Matrix4f::Identity();
		size_t IndexCount = (size_t)-1;
	};

}