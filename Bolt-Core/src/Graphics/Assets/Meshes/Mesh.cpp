#include "bltpch.h"
#include "Mesh.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	Mesh::Mesh(const VertexFactory& factory, bool calculateTangents) : Mesh(factory.GenerateVertices(), calculateTangents)
	{
	
	}

	Mesh::Mesh(MeshData&& data, bool calculateTangents) : Resource(),
		m_Data(std::move(data))
	{
		if (calculateTangents && m_Data.Vertices && m_Data.Indices && !m_Data.Vertices->IsMapped() && !m_Data.Indices->IsMapped())
		{
			ScopedVertexMap vertices = m_Data.Vertices->GetVertexBuffer(0).MapScoped(Access::ReadWrite);
			ScopedIndexMap indices = m_Data.Indices->GetIndexBuffer(0)->MapScoped(Access::ReadWrite);
			CalculateTangents(vertices, indices);
		}
	}

	const MeshData& Mesh::Data() const
	{
		return m_Data;
	}

	MeshData& Mesh::Data()
	{
		return m_Data;
	}

	int Mesh::VertexCount() const
	{
		return m_Data.Vertices->VertexCount();
	}

	int Mesh::IndexCount() const
	{
		return m_Data.Indices->IndexCount();
	}

	int Mesh::IndexBufferCount() const
	{
		return m_Data.Indices->IndexBufferCount();
	}

	int Mesh::TriangleCount() const
	{
		switch (m_Data.Vertices->GetRenderMode())
		{
		case RenderMode::Triangles:
			return IndexCount() / 3;
		default:
			return 0;
		}
	}

	std::unique_ptr<Resource> Mesh::Clone() const
	{
		BLT_PROFILE_FUNCTION();
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(MeshData{ m_Data.Vertices->Clone(), m_Data.Indices->Clone(), m_Data.Bounds });
		return mesh;
	}

	// Algorithm: Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software, 2001. http://terathon.com/code/tangent.html
	void Mesh::CalculateTangents(ScopedVertexMap& vertices, ScopedIndexMap& indices) const
	{
		BLT_PROFILE_FUNCTION();
		BLT_ASSERT(m_Data.Vertices->GetRenderMode() == RenderMode::Triangles, "Can only calculate tangents for triangle based models");
		BLT_ASSERT(indices.GetIndexCount() % 3 == 0, "Index count is not a multiple of 3");
		int triangleCount = indices.GetIndexCount() / 3;
		int vertexCount = vertices.GetVertexCount();

		std::vector<Vector3f> tangents;
		tangents.resize(vertexCount);

		IndexIterator<uint32_t> indexIt = indices.Begin<uint32_t>();

		for (int i = 0; i < triangleCount; i++)
		{
			uint32_t i0 = *indexIt++;
			uint32_t i1 = *indexIt++;
			uint32_t i2 = *indexIt++;

			DefaultVertexIterator vertex0 = vertices.DefaultBegin() + i0;
			DefaultVertexIterator vertex1 = vertices.DefaultBegin() + i1;
			DefaultVertexIterator vertex2 = vertices.DefaultBegin() + i2;

			const Vector3f& vp0 = vertex0.Position();
			const Vector3f& vp1 = vertex1.Position();
			const Vector3f& vp2 = vertex2.Position();

			const Vector2f& vt0 = vertex0.TexCoord();
			const Vector2f& vt1 = vertex1.TexCoord();
			const Vector2f& vt2 = vertex2.TexCoord();

			float x0 = vp1.x - vp0.x;
			float x1 = vp2.x - vp0.x;
			float y0 = vp1.y - vp0.y;
			float y1 = vp2.y - vp0.y;
			float z0 = vp1.z - vp0.z;
			float z1 = vp2.z - vp0.z;

			float s0 = vt1.x - vt0.x;
			float s1 = vt2.x - vt0.x;
			float t0 = vt1.y - vt0.y;
			float t1 = vt2.y - vt0.y;

			float r = 1.0f / (s0 * t1 - s1 * t0);
			Vector3f sDir((t1 * x0 - t0 * x1) * r, (t1 * y0 - t0 * y1) * r, (t1 * z0 - t0 * z1) * r);
			Vector3f tDir((s0 * x1 - s1 * x0) * r, (s0 * y1 - s1 * y0) * r, (s0 * z1 - s1 * z0) * r);

			tangents[i0] += sDir;
			tangents[i1] += sDir;
			tangents[i2] += sDir;
		}
		for (int i = 0; i < vertexCount; i++)
		{
			DefaultVertexIterator v = vertices.DefaultBegin() + i;
			const Vector3f& n = v.Normal();
			const Vector3f& t = tangents[i];
			v.Tangent() = (t - n * n.Dot(t)).Normalize();
		}
	}

	int Mesh::CalculateBufferIndex(int triangleIndex) const
	{
		BLT_PROFILE_FUNCTION();
		int current = 0;
		for (int i = 0; i < m_Data.Indices->IndexBufferCount(); i++)
		{
			if (current + (*m_Data.Indices)[0]->Size() > triangleIndex * 3)
			{
				return i;
			}
			current += (*m_Data.Indices)[0]->Size();
		}
		return -1;
	}

}