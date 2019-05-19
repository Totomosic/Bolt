#include "bltpch.h"

#include "EllipseFactory.h"

namespace Bolt
{

	EllipseFactory::EllipseFactory(float width, float height, int verticesPerRev, const Bolt::Color& color) : VertexFactory(),
		Width(width), Height(height), VerticesPerRev(verticesPerRev), Color(color)
	{
	
	}

	ModelData EllipseFactory::GenerateVertices() const
	{
		float w = Width / 2;
		float h = Height / 2;

		ModelData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(3 * VerticesPerRev));
		result.Bounds.MinX = -w;
		result.Bounds.MaxX = w;
		result.Bounds.MinY = -h;
		result.Bounds.MaxY = h;
		result.Bounds.MinZ = 0;
		result.Bounds.MaxZ = 0;

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->AddVertexBuffer(std::make_unique<VertexBuffer>((VerticesPerRev + 1) * layout.Stride(), layout));

		Vector4<byte> color = Color.ToBytes();

		{
			VertexMapping vMapping = result.Vertices->Map();
			IndexMapping iMapping = result.Indices->Map();
			VertexIterator iterator = vMapping.Begin();
			IndexIterator indices = iMapping.Begin();

			iterator[0] = Vector3f(0, 0, 0);
			iterator[1] = Vector3f(0, 0, 1);
			iterator[2] = Vector2f(0.5f, 0.5f);
			iterator[3] = color;
			iterator++;
			for (int i = 0; i < VerticesPerRev; i++)
			{
				float a = (float)PI * 2 / VerticesPerRev * i;
				float x = cos(a);
				float y = sin(a);
				iterator[0] = Vector3f(x * w, y * h, 0);
				iterator[1] = Vector3f(0, 0, 1);
				iterator[2] = Vector2f((x + 1) / 2, (y + 1) / 2);
				iterator[3] = color;
				iterator++;

				if (i != VerticesPerRev - 1)
				{
					*indices = 0;
					indices++;
					*indices = i + 1;
					indices++;
					*indices = i + 2;
					indices++;
				}
			}
			*indices = 0;
			indices++;
			*indices = VerticesPerRev;
			indices++;
			*indices = 1;
		}

		return result;
	}

}