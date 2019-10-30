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
		uint32_t IndexCount = (uint32_t)-1;
	};

}