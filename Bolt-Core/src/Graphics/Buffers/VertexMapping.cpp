#include "bltpch.h"
#include "VertexMapping.h"
#include "VertexArray.h"

namespace Bolt
{

	VertexMapping::VertexMapping(const std::vector<VertexMapping::MappingPtr>& mappedPtrs)
		: m_Array(nullptr), m_MappedPtrs(mappedPtrs), m_AttributeMap()
	{
		for (int i = 0; i < m_MappedPtrs.size(); i++)
		{
			MappingPtr& ptr = m_MappedPtrs[i];
			for (MappingAttribute& attrib : ptr.Attributes)
			{
				m_AttributeMap[attrib.Index] = i;
			}
		}
	}

	VertexMapping::VertexMapping(VertexMapping&& other)
		: m_Array(other.m_Array), m_MappedPtrs(std::move(other.m_MappedPtrs)), m_AttributeMap(std::move(other.m_AttributeMap))
	{
		other.m_Array = nullptr;
	}

	VertexMapping& VertexMapping::operator=(VertexMapping&& other)
	{
		const VertexArray* myArray = m_Array;
		m_Array = other.m_Array;
		m_MappedPtrs = std::move(other.m_MappedPtrs);
		m_AttributeMap = std::move(other.m_AttributeMap);
		other.m_Array = myArray;
		return *this;
	}

	VertexMapping::~VertexMapping()
	{
		if (m_Array != nullptr)
		{
			m_Array->SetMapped(false);
		}
	}

	VertexIterator VertexMapping::Begin() const
	{
		return GetVertex(0);
	}

	VertexIterator VertexMapping::End() const
	{
		return GetVertex(m_Array->VertexCount());
	}

	VertexIterator VertexMapping::GetVertex(int index) const
	{
		return VertexIterator(this, index);
	}

	bool VertexMapping::HasAttribute(int attributeIndex) const
	{
		return m_AttributeMap.find(attributeIndex) != m_AttributeMap.end();
	}

	void* VertexMapping::GetAttributePtr(int attributeIndex, int vertexIndex) const
	{
		BLT_ASSERT(HasAttribute(attributeIndex), "Attribute with index {} does not exist", attributeIndex);
		const MappingPtr& ptr = m_MappedPtrs.at(m_AttributeMap.at(attributeIndex));
		auto it = std::find_if(ptr.Attributes.begin(), ptr.Attributes.end(), [attributeIndex](const MappingAttribute& attrib)
			{
				return attrib.Index == attributeIndex;
			});
		const MappingAttribute& attrib = *it;
		byte* dataPtr = (byte*)ptr.Ptr;
		dataPtr += attrib.Offset;
		dataPtr += ptr.Stride * vertexIndex;
		return (void*)dataPtr;
	}

	void VertexMapping::SetVertexArray(const VertexArray* vertexArray)
	{
		m_Array = vertexArray;
	}

}