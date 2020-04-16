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

		std::unique_ptr<Resource> Clone() const override;

		void CalculateTangents(ScopedVertexMap& vertices, ScopedIndexMap& indices) const;

	private:
		int CalculateBufferIndex(int triangleIndex) const;

	};

}