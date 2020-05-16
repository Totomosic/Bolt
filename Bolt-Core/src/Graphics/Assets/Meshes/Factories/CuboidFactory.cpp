#include "bltpch.h"
#include "CuboidFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	CuboidFactory::CuboidFactory(float width, float height, float depth, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), Depth(depth), Color(color)
	{

	}

	MeshData CuboidFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		float w = Width / 2.0f;
		float h = Height / 2.0f;
		float d = Depth / 2.0f;

		MeshData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		uint32_t indices[6 * 6] = { 0, 1, 2, 0, 2, 3,  11, 10, 13, 11, 13, 12,  4, 5, 6, 4, 6, 7,  15, 14, 9, 15, 9, 8,  23, 16, 19, 23, 19, 20,  17, 22, 21, 17, 21, 18 };
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 6 * 6));
		result.Bounds.Min.x = -w;
		result.Bounds.Max.x = w;
		result.Bounds.Min.y = -h;
		result.Bounds.Max.y = h;
		result.Bounds.Min.z = -d;
		result.Bounds.Max.z = d;

		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = result.Vertices->CreateVertexBuffer(24 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		{
			ScopedVertexMap vertexMap = buffer.MapScoped(Access::Write);
			DefaultVertexIterator iterator = vertexMap.DefaultBegin();
			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(0, 0, -1);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;

			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			++iterator;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			++iterator;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			++iterator;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			++iterator;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(1, 0, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, -1);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			++iterator;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(-1, 0, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(0, 0, 1);
			++iterator;

			iterator.Position() = Vector3f(-w, h, d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, -h, d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, h, d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, h, -d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(1, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(w, -h, -d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(1, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, -h, -d);
			iterator.Normal() = Vector3f(0, -1, 0);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(-1, 0, 0);
			++iterator;
			iterator.Position() = Vector3f(-w, h, -d);
			iterator.Normal() = Vector3f(0, 1, 0);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			iterator.Tangent() = Vector3f(1, 0, 0);
			++iterator;
		}

		return result;
	}

}