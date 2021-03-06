#include "bltpch.h"
#include "EllipseFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	EllipseFactory::EllipseFactory(float width, float height, int verticesPerRev, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), VerticesPerRev(verticesPerRev), Color(color)
	{
	
	}

	MeshData EllipseFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		float w = Width / 2;
		float h = Height / 2;

		MeshData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		IndexBuffer& indexBuffer = result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(3 * VerticesPerRev));
		result.Bounds.Min.x = -w;
		result.Bounds.Max.x = w;
		result.Bounds.Min.y = -h;
		result.Bounds.Max.y = h;
		result.Bounds.Min.z = 0;
		result.Bounds.Max.z = 0;

		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = result.Vertices->AddVertexBuffer(std::make_unique<VertexBuffer>((VerticesPerRev + 1) * layout.Stride(), layout));

		Vector4<byte> color = Color.ToBytes();

		{
			ScopedVertexMap vMapping = buffer.MapScoped(Access::Write);
			ScopedIndexMap iMapping = indexBuffer.MapScoped(Access::Write);
			DefaultVertexIterator iterator = vMapping.DefaultBegin();
			IndexIterator<uint32_t> indices = iMapping.Begin<uint32_t>();

			iterator.Position() = Vector3f(0, 0, 0);
			iterator.Normal() = Vector3f(0, 0, 1);
			iterator.TexCoord() = Vector2f(0.5f, 0.5f);
			iterator.Color() = color;
			++iterator;
			for (int i = 0; i < VerticesPerRev; i++)
			{
				float a = (float)PI * 2 / VerticesPerRev * i;
				float x = cos(a);
				float y = sin(a);
				iterator.Position() = Vector3f(x * w, y * h, 0);
				iterator.Normal() = Vector3f(0, 0, 1);
				iterator.TexCoord() = Vector2f((x + 1) / 2, (y + 1) / 2);
				iterator.Color() = color;
				++iterator;

				if (i != VerticesPerRev - 1)
				{
					*indices = 0;
					++indices;
					*indices = i + 1;
					++indices;
					*indices = i + 2;
					++indices;
				}
			}
			*indices = 0;
			++indices;
			*indices = VerticesPerRev;
			++indices;
			*indices = 1;
		}

		return result;
	}

}