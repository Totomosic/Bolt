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

		std::vector<Triangle> GetTriangles() const;
		Triangle GetTriangle(int index) const;

		template<size_t VertexCount>
		std::vector<Face<VertexCount>> GetFaces() const
		{
			std::vector<Face<VertexCount>> result;
			if (m_Data.Vertices->GetRenderMode() == RenderMode::Triangles)
			{
				constexpr int multiple = ((VertexCount + 3 - 1) / 3) * 3;
				IndexIterator<> it = m_Data.Indices.Begin();
				int indexCount = m_Data.Indices.Descriptor().IndexCount();
				int currentIndex = 0;				
				while (currentIndex < indexCount)
				{
					Face<VertexCount> currentFace;
					int vCount = 0;
					std::vector<VertexIterator*> addedIterators;
					while (vCount < VertexCount)
					{
						VertexIterator v = m_Data.Vertices->GetVertex(*it);
						it++;
						currentIndex++;
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
							currentFace.Vertices[vCount] = std::move(v);
							addedIterators.push_back(&currentFace.Vertices[vCount]);
							vCount++;
						}
					}
					result.push_back(std::move(currentFace));
				}
			}
			return result;
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