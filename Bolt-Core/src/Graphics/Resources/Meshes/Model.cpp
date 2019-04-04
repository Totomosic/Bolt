#include "Types.h"

#include "Model.h"

namespace Bolt
{

	Model::Model(const VertexFactory& factory) : Model(factory.GenerateVertices())
	{
	
	}

	Model::Model(ModelData&& data) : Resource(),
		m_Data(std::move(data))
	{
		
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
		return { Data().Vertices->Map(), Data().Indices->Map() };
	}

	std::unique_ptr<Resource> Model::Clone() const
	{
		std::unique_ptr<Model> model = std::make_unique<Model>(ModelData{ m_Data.Vertices->Clone(), m_Data.Indices->Clone(), m_Data.Bounds });
		return model;
	}

	int Model::CalculateBufferIndex(int triangleIndex) const
	{
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