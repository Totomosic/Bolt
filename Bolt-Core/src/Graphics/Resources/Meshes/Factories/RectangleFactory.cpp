#include "bltpch.h"
#include "RectangleFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	RectangleFactory::RectangleFactory(float width, float height, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), Color(color)
	{
		
	}

	ModelData RectangleFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		float w = Width / 2.0f;
		float h = Height / 2.0f;

		ModelData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 6));
		result.Bounds.Min.x = -w;
		result.Bounds.Max.x = w;
		result.Bounds.Min.y = -h;
		result.Bounds.Max.y = h;
		result.Bounds.Min.z = 0;
		result.Bounds.Max.z = 0;

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(4 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		{
			VertexMapping mapping = result.Vertices->Map();
			VertexIterator iterator = mapping.Begin();
			iterator.Position() = Vector3f(-w, h, 0);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator++;
			iterator.Position() = Vector3f(-w, -h, 0);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator++;
			iterator.Position() = Vector3f(w, -h, 0);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator++;
			iterator.Position() = Vector3f(w, h, 0);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
		}

		return result;
	}

}