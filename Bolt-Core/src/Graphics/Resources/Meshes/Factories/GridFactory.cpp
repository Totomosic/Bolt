#include "GridFactory.h"

namespace Bolt
{

	GridFactory::GridFactory(float width, float depth, int xVertices, int zVertices, const Bolt::Color& color) : VertexFactory(),
		Width(width), Depth(depth), xVertices(xVertices), zVertices(zVertices), Color(color)
	{

	}

	ModelData GridFactory::GenerateVertices() const
	{
		ModelData result;
		result.Vertices = std::make_unique<VertexArray>(RenderMode::Triangles);
		int vertexCount = xVertices * zVertices;
		result.Indices.AddIndexBuffer(std::make_unique<IndexBuffer>((xVertices - 1) * (zVertices - 1) * 6));		
		result.Bounds.MinX = -Width / 2;
		result.Bounds.MaxX = Width / 2;
		result.Bounds.MinY = 0;
		result.Bounds.MaxY = 0;
		result.Bounds.MinZ = -Depth / 2;
		result.Bounds.MaxZ = Depth / 2;

		BufferLayout layout = BufferLayout::Default();
		result.Vertices->CreateVertexBuffer(vertexCount * layout.Size(), layout);

		float cellWidth = Width / (xVertices - 1);
		float cellDepth = Depth / (zVertices - 1);

		Vector4<byte> color = Color.ToBytes();

		{
			VertexIterator iterator = result.Vertices->Begin();
			IndexIterator<uint> indices = result.Indices.Begin();
			for (int z = 0; z < zVertices; z++)
			{
				for (int x = 0; x < xVertices; x++)
				{
					iterator[0] = Vector3f(-Width / 2 + x * cellWidth, 0.0f, -Depth / 2 + z * cellDepth);
					iterator[1] = Vector3f(0.0f, 1.0f, 0.0f);
					iterator[2] = Vector2f((float)x / (xVertices - 1), (float)z / (zVertices - 1));
					iterator[3] = color;
					iterator++;

					if (x < xVertices - 1 && z < zVertices - 1)
					{
						*indices = x + xVertices * z;
						indices++;
						*indices = x + xVertices * (z + 1);
						indices++;
						*indices = (x + 1) + xVertices * (z + 1);
						indices++;
						*indices = x + xVertices * z;
						indices++;
						*indices = (x + 1) + xVertices * (z + 1);
						indices++;
						*indices = (x + 1) + xVertices * z;
						indices++;
					}
				}
			}
		}

		return result;
	}

}