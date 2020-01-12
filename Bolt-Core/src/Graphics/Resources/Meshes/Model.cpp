#include "bltpch.h"
#include "Model.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	Model::Model(const VertexFactory& factory, bool calculateTangents) : Model(factory.GenerateVertices(), calculateTangents)
	{
	
	}

	Model::Model(ModelData&& data, bool calculateTangents) : Resource(),
		m_Data(std::move(data))
	{
		if (calculateTangents && m_Data.Vertices && m_Data.Indices)
		{
			ModelMapping mapping = Map();
			CalculateTangents(mapping);
		}
	}

	const ModelData& Model::Data() const
	{
		return m_Data;
	}

	ModelData& Model::Data()
	{
		return m_Data;
	}

	int Model::VertexCount() const
	{
		return m_Data.Vertices->VertexCount();
	}

	int Model::IndexCount() const
	{
		return m_Data.Indices->IndexCount();
	}

	int Model::IndexBufferCount() const
	{
		return m_Data.Indices->IndexBufferCount();
	}

	int Model::TriangleCount() const
	{
		switch (m_Data.Vertices->GetRenderMode())
		{
		case RenderMode::Triangles:
			return IndexCount() / 3;
		default:
			return 0;
		}
	}

	ModelMapping Model::Map() const
	{
		BLT_PROFILE_FUNCTION();
		return { Data().Vertices->Map(), Data().Indices->Map() };
	}

	std::unique_ptr<Resource> Model::Clone() const
	{
		BLT_PROFILE_FUNCTION();
		std::unique_ptr<Model> model = std::make_unique<Model>(ModelData{ m_Data.Vertices->Clone(), m_Data.Indices->Clone(), m_Data.Bounds });
		return model;
	}

	// Algorithm: Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software, 2001. http://terathon.com/code/tangent.html
	void Model::CalculateTangents(ModelMapping& mapping) const
	{
		BLT_PROFILE_FUNCTION();
		BLT_ASSERT(m_Data.Vertices->GetRenderMode() == RenderMode::Triangles, "Can only calculate tangents for triangle based models");
		BLT_ASSERT(mapping.IndexMap.IndexCount() % 3 == 0, "Index count is not a multiple of 3");
		IndexIterator indices = mapping.IndexMap.Begin();
		int triangleCount = mapping.IndexMap.IndexCount() / 3;
		int vertexCount = mapping.VertexMap.VertexCount();

		std::vector<Vector3f> tangents;
		tangents.resize(vertexCount);

		for (int i = 0; i < triangleCount; i++)
		{
			uint32_t i0 = *indices++;
			uint32_t i1 = *indices++;
			uint32_t i2 = *indices++;

			VertexIterator vertex0 = mapping.VertexMap.GetVertex(i0);
			VertexIterator vertex1 = mapping.VertexMap.GetVertex(i1);
			VertexIterator vertex2 = mapping.VertexMap.GetVertex(i2);

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
			VertexIterator v = mapping.VertexMap.GetVertex(i);
			const Vector3f& n = v.Normal();
			const Vector3f& t = tangents[i];
			v.Tangent() = (t - n * n.Dot(t)).Normalize();
		}
	}

	int Model::CalculateBufferIndex(int triangleIndex) const
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