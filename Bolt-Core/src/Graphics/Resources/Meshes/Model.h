#pragma once
#include "Factories\VertexFactory.h"
#include "..\Resource.h"
#include "Face.h"

namespace Bolt
{

	class BLT_API Model : public Resource
	{
	private:
		ModelData m_Data;

	public:
		Model(const VertexFactory& factory);
		Model(ModelData&& data);
		Model(const Model& other) = delete;
		Model(Model&& other) = default;
		Model& operator=(const Model& other) = delete;
		Model& operator=(Model&& other) = default;
		~Model() = default;

		const ModelData& Data() const;
		ModelData& Data();
		int VertexCount() const;
		int IndexCount() const;
		int IndexBufferCount() const;
		int TriangleCount() const;

		std::unique_ptr<Resource> Clone() const override;

		std::vector<Triangle> GetTriangles() const;
		Triangle GetTriangle(int index) const;

		template<size_t VertexCount>
		std::vector<Face<VertexCount>> GetFaces() const
		{
			BLT_ASSERT(false, "Unable to get faces with " + std::to_string(VertexCount) + " vertices");
			return std::vector<Face<VertexCount>>();
		}

		template<>
		std::vector<Face<3>> GetFaces() const
		{
			std::vector<Face<3>> faces;
			IndexIterator<> it = Data().Indices.Begin();
			int indexCount = Data().Indices.IndexCount();
			for (int i = 0; i < indexCount; i += 3)
			{
				Face<3> currentFace;
				BLT_INFO(*it);
				currentFace.Vertices[0] = Data().Vertices->GetVertex(*it);
				it++;
				currentFace.Vertices[1] = Data().Vertices->GetVertex(*it);
				it++;
				currentFace.Vertices[2] = Data().Vertices->GetVertex(*it);
				it++;
				faces.push_back(std::move(currentFace));
			}
			return faces;
		}

		template<size_t VertexCount>
		Face<VertexCount> GetFace(int index) const
		{
			if (m_Data.Vertices->GetRenderMode() == RenderMode::Triangles)
			{
				constexpr int multiple = ((VertexCount + 3 - 1) / 3) * 3;
				int	vertexIndex = index * multiple;
				IndexIterator<> it = m_Data.Indices.GetIterator(vertexIndex);
				Face<VertexCount> result;
				std::vector<VertexIterator*> addedIterators;
				int count = 0;
				while (count < VertexCount)
				{
					VertexIterator v = m_Data.Vertices->GetVertex(*it);
					it++;
					bool found = false;
					for (VertexIterator* i : addedIterators)
					{
						if (*i == v)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						result.Vertices[count] = std::move(v);
						addedIterators.push_back(&result.Vertices[count]);
						count++;
					}					
				}
				return result;
			}
			BLT_ASSERT(false, "RenderMode must be Triangles");
			return std::move(*(Face<VertexCount>*)(nullptr));
		}

	private:
		int CalculateBufferIndex(int triangleIndex) const;

	};

}