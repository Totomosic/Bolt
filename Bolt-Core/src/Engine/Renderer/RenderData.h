#pragma once
#include "BoltLib/Math/Matrices/Matrix4.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Assets/Meshes/Materials/Material.h"

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

	struct BLT_API MaterialPair
	{
	public:
		const Bolt::Material* Material;
		std::vector<RenderData> Renderables;
	};

	struct BLT_API RenderGroup
	{
		const ShaderInstance* Shader;
		std::vector<MaterialPair> RenderData;
	};

}