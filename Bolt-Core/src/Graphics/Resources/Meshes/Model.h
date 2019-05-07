#pragma once
#include "Factories/VertexFactory.h"
#include "../Resource.h"
#include "Face.h"

namespace Bolt
{

	struct BLT_API ModelMapping
	{
	public:
		VertexMapping VertexMap;
		IndexMapping IndexMap;
	};

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

		ModelMapping Map() const;

		std::unique_ptr<Resource> Clone() const override;

		template<size_t VertexCount>
		std::vector<Face<VertexCount>> GetFaces(ModelMapping& mapping) const
		{
			BLT_ASSERT(false, "Unable to get faces with " + std::to_string(VertexCount) + " vertices");
			return std::vector<Face<VertexCount>>();
		}

		template<>
		std::vector<Face<3>> GetFaces(ModelMapping& mapping) const
		{
			std::vector<Face<3>> faces;
			IndexIterator it = mapping.IndexMap.Begin();
			int indexCount = Data().Indices->IndexCount();
			for (int i = 0; i < indexCount; i += 3)
			{
				Face<3> currentFace;
				currentFace.Vertices[0] = mapping.VertexMap.GetVertex(*it++);
				currentFace.Vertices[1] = mapping.VertexMap.GetVertex(*it++);
				currentFace.Vertices[2] = mapping.VertexMap.GetVertex(*it++);
				faces.push_back(std::move(currentFace));
			}
			return faces;
		}

		template<>
		std::vector<Face<4>> GetFaces(ModelMapping& mapping) const
		{
			std::vector<Face<4>> faces;
			IndexIterator it = mapping.IndexMap.Begin();
			int indexCount = Data().Indices->IndexCount();
			for (int i = 0; i < indexCount; i += 6)
			{
				Face<4> currentFace;
				currentFace.Vertices[0] = mapping.VertexMap.GetVertex(*it++);
				currentFace.Vertices[1] = mapping.VertexMap.GetVertex(*it++);
				currentFace.Vertices[2] = mapping.VertexMap.GetVertex(*it++);
				it += 2;
				currentFace.Vertices[3] = mapping.VertexMap.GetVertex(*it++);
				faces.push_back(std::move(currentFace));
			}
			return faces;
		}

		template<size_t VertexCount>
		Face<VertexCount> GetFace(ModelMapping& mapping, int index) const
		{
			BLT_ASSERT(false, "Unable to get faces with {} vertices", VertexCount);
			return Face<VertexCount>();
		}

		template<>
		Face<3> GetFace(ModelMapping& mapping, int index) const
		{
			IndexIterator it = mapping.IndexMap.Begin() + index * 3;
			Face<3> result;
			result.Vertices[0] = mapping.VertexMap.GetVertex(*it++);
			result.Vertices[1] = mapping.VertexMap.GetVertex(*it++);
			result.Vertices[2] = mapping.VertexMap.GetVertex(*it++);
			return result;
		}

		template<>
		Face<4> GetFace(ModelMapping& mapping, int index) const
		{
			IndexIterator it = mapping.IndexMap.Begin() + index * 6;
			Face<4> result;
			result.Vertices[0] = mapping.VertexMap.GetVertex(*it++);
			result.Vertices[1] = mapping.VertexMap.GetVertex(*it++);
			result.Vertices[2] = mapping.VertexMap.GetVertex(*it++);
			it += 2;
			result.Vertices[3] = mapping.VertexMap.GetVertex(*it++);
			return result;
		}

	private:
		int CalculateBufferIndex(int triangleIndex) const;

	};

}