#pragma once
#include "Factories/VertexFactory.h"
#include "../Resource.h"
#include "Face.h"

namespace Bolt
{

	class BLT_API Mesh : public Resource
	{
	private:
		MeshData m_Data;

	public:
		Mesh(const VertexFactory& factory, bool calculateTangents = true);
		Mesh(MeshData&& data, bool calculateTangents = true);

		const MeshData& Data() const;
		MeshData& Data();
		int VertexCount() const;
		int IndexCount() const;
		int IndexBufferCount() const;
		int TriangleCount() const;

		std::unique_ptr<Resource> Clone() const override;

		void CalculateTangents(ScopedVertexMap& vertices, ScopedIndexMap& indices) const;

	private:
		int CalculateBufferIndex(int triangleIndex) const;

	};

}