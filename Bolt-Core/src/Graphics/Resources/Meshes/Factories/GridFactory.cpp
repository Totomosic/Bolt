#include "bltpch.h"
#include "GridFactory.h"

#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	GridFactory::GridFactory(float width, float depth, int xVertices, int zVertices, const Bolt::Color& color) : VertexFactory(),
		Width(width), Depth(depth), xVertices(xVertices), zVertices(zVertices), Color(color)
	{

	}

	ModelData GridFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		ModelData result;
		result.Vertices = std::make_unique<VertexArray>(RenderMode::Triangles);
		result.Indices = std::make_unique<IndexArray>();
		int vertexCount = xVertices * zVertices;
		result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>((xVertices - 1) * (zVertices - 1) * 6));		
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
			VertexMapping vMapping = result.Vertices->Map();
			IndexMapping iMapping = result.Indices->Map();
			VertexIterator iterator = vMapping.Begin();
			IndexIterator indices = iMapping.Begin();
			for (int z = 0; z < zVertices; z++)
			{
				for (int x = 0; x < xVertices; x++)
				{
					iterator.Position() = Vector3f(-Width / 2 + x * cellWidth, 0.0f, -Depth / 2 + z * cellDepth);
					iterator.Normal() = Vector3f(0.0f, 1.0f, 0.0f);
					iterator.TexCoord() = Vector2f((float)x / (xVertices - 1), 1 - (float)z / (zVertices - 1));
					iterator.Color() = color;
					iterator.Tangent() = Vector3f(1, 0, 0);
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