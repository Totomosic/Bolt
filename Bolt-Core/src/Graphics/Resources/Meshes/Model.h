#pragma once
#include "Factories/VertexFactory.h"
#include "../Resource.h"
#include "Face.h"

namespace Bolt
{

	class BLT_API Model : public Resource
	{
	private:
		ModelData m_Data;

	public:
		Model(const VertexFactory& factory, bool calculateTangents = true);
		Model(ModelData&& data, bool calculateTangents = true);

		const ModelData& Data() const;
		ModelData& Data();
		int VertexCount() const;
		int IndexCount() const;
		int IndexBufferCount() const;
		int TriangleCount() const;

		ModelMapping Map() const;

		std::unique_ptr<Resource> Clone() const override;

		void CalculateTangents(ModelMapping& mapping) const;

		template<uint32_t VertexCount>
		std::vector<Face<VertexCount>> GetFaces(ModelMapping& mapping) const
		{
			BLT_ASSERT(false, "Unable to get faces with " + std::to_string(VertexCount) + " vertices");
			return std::vector<Face<VertexCount>>();
		}

		template<uint32_t VertexCount>
		Face<VertexCount> GetFace(ModelMapping& mapping, int index) const
		{
			BLT_ASSERT(false, "Unable to get faces with {} vertices", VertexCount);
			return Face<VertexCount>();
		}

	private:
		int CalculateBufferIndex(int triangleIndex) const;

	};

	template<>
	inline std::vector<Face<3>> Model::GetFaces(ModelMapping& mapping) const
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
	inline std::vector<Face<4>> Model::GetFaces(ModelMapping& mapping) const
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

	template<>
	inline Face<3> Model::GetFace(ModelMapping& mapping, int index) const
	{
		IndexIterator it = mapping.IndexMap.Begin() + index * 3;
		Face<3> result;
		result.Vertices[0] = mapping.VertexMap.GetVertex(*it++);
		result.Vertices[1] = mapping.VertexMap.GetVertex(*it++);
		result.Vertices[2] = mapping.VertexMap.GetVertex(*it++);
		return result;
	}

	template<>
	inline Face<4> Model::GetFace(ModelMapping& mapping, int index) const
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

}