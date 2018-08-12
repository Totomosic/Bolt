#pragma once
#include "VertexBuffer.h"

namespace Bolt
{

	class BLT_API ArrayDescriptor
	{
	private:
		std::unordered_map<int, VertexBuffer*> m_Attributes;

	public:
		ArrayDescriptor() = default;
		ArrayDescriptor(const ArrayDescriptor& other) = default;
		ArrayDescriptor(ArrayDescriptor&& other) = default;
		ArrayDescriptor& operator=(const ArrayDescriptor& other) = default;
		ArrayDescriptor& operator=(ArrayDescriptor&& other) = default;
		~ArrayDescriptor() = default;

		int AttributeCount() const;
		int LowestAttribute() const;
		bool HasAttribute(int index) const;
		VertexBuffer* GetAttribute(int attributeIndex) const;

		friend class VertexArray;

	private:
		void AddVertexBuffer(VertexBuffer* buffer);

	};

}