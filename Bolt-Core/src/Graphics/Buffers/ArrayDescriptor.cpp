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

	void ArrayDescriptor::AddVertexBuffer(VertexBuffer* buffer)
	{
		const BufferLayout& layout = buffer->Layout();
		for (const BufferLayout::VertexAttribute& attrib : layout.GetAttributes())
		{
			m_Attributes[attrib.Index] = buffer;
		}
	}

}