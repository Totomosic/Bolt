#include "bltpch.h"
#include "SphereFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	SphereFactory::SphereFactory(float radius, int sectorCount, const Bolt::Color& color) : VertexFactory(),
		Radius(radius), SectorCount(sectorCount), Color(color)
	{
	
	}

	MeshData SphereFactory::GenerateVertices() const
	{
		BLT_PROFILE_FUNCTION();
		MeshData result;
		result.Vertices = std::make_unique<VertexArray>();
		result.Indices = std::make_unique<IndexArray>();
		IndexBuffer& indexBuffer = result.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(6 * (SectorCount * (SectorCount - 1))));
		result.Bounds.Min.x = -Radius;
		result.Bounds.Max.x = Radius;
		result.Bounds.Min.y = -Radius;
		result.Bounds.Max.y = Radius;
		result.Bounds.Min.z = -Radius;
		result.Bounds.Max.z = Radius;

		BufferLayout layout = BufferLayout::Default();
		VertexBuffer& buffer = result.Vertices->AddVertexBuffer(std::make_unique<VertexBuffer>(((SectorCount + 1) * (SectorCount + 1)) * layout.Stride(), layout));

		Vector4<byte> color = Color.ToBytes();

		{
			ScopedVertexMap vMapping = buffer.MapScoped(Access::Write);
			ScopedIndexMap iMapping = indexBuffer.MapScoped(Access::Write);
			DefaultVertexIterator iterator = vMapping.DefaultBegin();
			IndexIterator<uint32_t> indices = iMapping.Begin<uint32_t>();

			float x, y, z, xy;                              // vertex position
			float nx, ny, nz, lengthInv = 1.0f / Radius;    // vertex normal
			float s, t;                                     // vertex texCoord

			float sectorStep = 2 * PI / SectorCount;
			float stackStep = PI / SectorCount;
			float sectorAngle, stackAngle;

			for (int i = 0; i <= SectorCount; i++)
			{
				stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
				xy = Radius * cosf(stackAngle);             // r * cos(u)
				z = Radius * sinf(stackAngle);              // r * sin(u)

				// add (sectorCount+1) vertices per stack
				// the first and last vertices have same position and normal, but different tex coords
				for (int j = 0; j <= SectorCount; j++)
				{
					sectorAngle = j * sectorStep;           // starting from 0 to 2pi

					// vertex position (x, y, z)
					x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
					y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
					iterator.Position() = Vector3f(x, y, z);

					// normalized vertex normal (nx, ny, nz)
					nx = x * lengthInv;
					ny = y * lengthInv;
					nz = z * lengthInv;
					iterator.Normal() = Vector3f(nx, ny, nz);

					// vertex tex coord (s, t) range between [0, 1]
					s = (float)j / SectorCount;
					t = (float)i / SectorCount;
					iterator.TexCoord() = Vector2f(s, t);
					iterator.Color() = color;
					++iterator;
				}
			}

			int k1, k2;
			for (int i = 0; i < SectorCount; ++i)
			{
				k1 = i * (SectorCount + 1);     // beginning of current stack
				k2 = k1 + SectorCount + 1;      // beginning of next stack

				for (int j = 0; j < SectorCount; ++j, ++k1, ++k2)
				{
					// 2 triangles per sector excluding first and last stacks
					// k1 => k2 => k1+1
					if (i != 0)
					{
						*indices = (k1);
						++indices;
						*indices = (k2);
						++indices;
						*indices = (k1 + 1);
						++indices;
					}

					// k1+1 => k2 => k2+1
					if (i != (SectorCount - 1))
					{
						*indices = (k1 + 1);
						++indices;
						*indices = (k2);
						++indices;
						*indices = (k2 + 1);
						++indices;
					}
				}
			}
		}
		return result;
	}

}