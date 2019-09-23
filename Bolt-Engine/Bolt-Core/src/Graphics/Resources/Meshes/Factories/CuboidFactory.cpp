#include "bltpch.h"

#include "CuboidFactory.h"

namespace Bolt
{

	CuboidFactory::CuboidFactory(float width, float height, float depth, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), Depth(depth), Color(color)
	{

	}

	ModelData CuboidFactory::GenerateVertices() const
	{
		float w = Width / 2.0f;
		float h = Height / 2.0f;
		float d = Depth / 2.0f;

		ModelData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		uint32_t indices[6 * 6] = { 0, 1, 2, 0, 2, 3,  11, 10, 13, 11, 13, 12,  4, 5, 6, 4, 6, 7,  15, 14, 9, 15, 9, 8,  23, 16, 19, 23, 19, 20,  17, 22, 21, 17, 21, 18 };
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 6 * 6));
		result.Bounds.MinX = -w;
		result.Bounds.MaxX = w;
		result.Bounds.MinY = -h;
		result.Bounds.MaxY = h;
		result.Bounds.MinZ = -d;
		result.Bounds.MaxZ = d;

		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = result.Vertices->CreateVertexBuffer(24 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		{
			VertexMapping mapping = result.Vertices->Map();
			VertexIterator iterator = mapping.Begin();
			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;

			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			iterator++;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			iterator++;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			iterator++;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			iterator++;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			iterator++;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			iterator++;

			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			iterator++;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			iterator++;
		}

		return result;
	}

}