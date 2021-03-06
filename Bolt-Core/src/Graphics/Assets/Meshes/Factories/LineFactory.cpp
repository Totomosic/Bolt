#include "bltpch.h"
#include "LineFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	LineFactory::LineFactory(const Vector3f& direction, float length, const Bolt::Color& color) : VertexFactory(),
		Direction(direction), Length(length), Color(color)
	{

	}

	MeshData LineFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		MeshData result;
		result.Vertices = std::make_unique<VertexArray>(RenderMode::Lines);
		result.Indices = std::make_unique<IndexArray>();
		uint32_t indices[2] = { 0, 1 };
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(indices, 2));
		Vector3f half = Direction * Length / 2.0f;
		result.Bounds.Min.x = -abs(half.x);
		result.Bounds.Max.x = abs(half.x);
		result.Bounds.Min.y = -abs(half.y);
		result.Bounds.Max.y = abs(half.y);
		result.Bounds.Min.z = -abs(half.z);
		result.Bounds.Max.z = abs(half.z);
						 
		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = result.Vertices->CreateVertexBuffer(2 * layout.Size(), layout);

		Vector4<byte> color = Color.ToBytes();

		{
			ScopedVertexMap mapping = buffer.MapScoped(Access::Write);
			DefaultVertexIterator iterator = mapping.DefaultBegin();
			iterator.Position() = -half;
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 1);
			iterator.Color() = color;
			++iterator;
			iterator.Position() = half;
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0, 0);
			iterator.Color() = color;
		}

		return result;
	}

}