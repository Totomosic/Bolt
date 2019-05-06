#pragma once
#include "Core/Math/Matrices/Matrix4.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexBuffer.h"

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