#include "Types.h"

#include "ArrayDescriptor.h"

namespace Bolt
{

	int ArrayDescriptor::AttributeCount() const
	{
		return m_Attributes.size();
	}

	int ArrayDescriptor::LowestAttribute() const
	{
		if (m_Attributes.size() == 0)
		{
			return -1;
		}
		int lowest = 10000000;
		for (const auto& pair : m_Attributes)
		{
			if (pair.first < lowest)
			{
				lowest = pair.first;
			}
		}
		return lowest;
	}

	bool ArrayDescriptor::HasAttribute(int index) const
	{
		return m_Attributes.find(index) != m_Attributes.end();
	}

	VertexBuffer* ArrayDescriptor::GetAttribute(int attributeIndex) const
	{
		return m_Attributes.at(attributeIndex);
	}

	VertexMapping ArrayDescriptor::GetMapping() const
	{
		std::vector<VertexMapping::MappingPtr> mappedPtrs;
		for (const auto& pair : m_Attributes)
		{
			VertexMapping::MappingPtr mappingPtr;
			const BufferLayout& layout = pair.second->Layout();
			mappingPtr.Ptr = pair.second->Map(Access::ReadWrite);
			mappingPtr.Stride = layout.Stride();
			for (const BufferLayout::VertexAttribute& attrib : layout.GetAttributes())
			{
				VertexMapping::MappingAttribute attribute;
				attribute.Index = attrib.Index;
				attribute.Offset = attrib.Offset;
				attribute.Size = attrib.Count * 4; // TODO: Fix
				mappingPtr.Attributes.push_back(attribute);
			}
			mappedPtrs.push_back(mappingPtr);
		}
		return VertexMapping(mappedPtrs);
	}

	void ArrayDescriptor::AddVertexBuffer(VertexBuffer* buffer)
	{
		const BufferLayout& layout = buffer->Layout();
		for (const BufferLayout::VertexAttribute& attrib : layout.GetAttributes())
		{
			m_Attributes[attrib.Index] = buffer;
		}
	}

	void ArrayDescriptor::UnmapAll() const
	{
		for (const auto& pair : m_Attributes)
		{
			pair.second->Unmap();
		}
	}

}