#include "bltpch.h"

#include "LineFactory.h"

namespace Bolt
{

	LineFactory::LineFactory(const Vector3f& direction, float length, const Bolt::Color& color) : VertexFactory(),
		Direction(direction), Length(length), Color(color)
	{

	}

	ModelData LineFactory::GenerateVertices() const
	{
		ModelData result;
		result.Vertices = std::make_unique<VertexArray>(RenderMode::Lines);
		result.Indices = std::make_unique<IndexArray>();
		uint32_t indices[2] = { 0, 1 };
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 2));
		Vector3f half = Direction * Length / 2.0f;
		result.Bounds.MinX = -abs(half.x);
		result.Bounds.MaxX = abs(half.x);
		result.Bounds.MinY = -abs(half.y);
		result.Bounds.MaxY = abs(half.y);
		result.Bounds.MinZ = -abs(half.z);
		result.Bounds.MaxZ = abs(half.z);

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(2 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		{
			VertexMapping mapping = result.Vertices->Map();
			VertexIterator iterator = mapping.Begin();
			iterator.Position() = -half;
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator++;
			iterator.Position() = half;
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
		}

		return result;
	}

}