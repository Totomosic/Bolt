#pragma once
#include "VertexBuffer.h"
#include "VertexMapping.h"

namespace Bolt
{

	class BLT_API ArrayDescriptor
	{
	private:
		std::unordered_map<int, VertexBuffer*> m_Attributes;

	public:
		ArrayDescriptor() = default;

		int AttributeCount() const;
		int LowestAttribute() const;
		bool HasAttribute(int index) const;
		VertexBuffer* GetAttribute(int attributeIndex) const;

		VertexMapping GetMapping() const;

		friend class VertexArray;

	private:
		void AddVertexBuffer(VertexBuffer* buffer);
		void UnmapAll() const;

	};

}