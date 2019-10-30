#include "bltpch.h"
#include "VertexMapping.h"
#include "VertexArray.h"

namespace Bolt
{

	VertexMapping::VertexMapping(const std::vector<VertexMapping::MappingPtr>& mappedPtrs)
		: m_Array(nullptr), m_MappedPtrs(mappedPtrs), m_Attributes()
	{

	}

	VertexMapping::VertexMapping(VertexMapping&& other)
		: m_Array(other.m_Array), m_MappedPtrs(std::move(other.m_MappedPtrs)), m_Attributes(std::move(other.m_Attributes))
	{
		other.m_Array = nullptr;
	}

	VertexMapping& VertexMapping::operator=(VertexMapping&& other)
	{
		const VertexArray* myArray = m_Array;
		m_Array = other.m_Array;
		m_MappedPtrs = std::move(other.m_MappedPtrs);
		m_Attributes = std::move(other.m_Attributes);
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

	int VertexMapping::VertexCount() const
	{
		return m_Array->VertexCount();
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
		auto it = std::find_if(m_MappedPtrs.begin(), m_MappedPtrs.end(), [attributeIndex](const MappingPtr& ptr)
			{
				auto it = std::find_if(ptr.Attributes.begin(), ptr.Attributes.end(), [attributeIndex](const MappingAttribute& attr)
					{
						return attr.Index == attributeIndex;
					});
				return it != ptr.Attributes.end();
			});
		return it != m_MappedPtrs.end();
	}

	std::pair<const VertexMapping::MappingPtr&, const VertexMapping::MappingAttribute&> VertexMapping::GetMappingInfo(int attribIndex) const
	{
		for (const MappingPtr& ptr : m_MappedPtrs)
		{
			auto it = std::find_if(ptr.Attributes.begin(), ptr.Attributes.end(), [attribIndex](const MappingAttribute& attr)
				{
					return attr.Index == attribIndex;
				});
			return { ptr, *it };
		}
		BLT_ASSERT(false, "Unable to find mapping attribute");
		return { *(MappingPtr*)nullptr, *(MappingAttribute*)nullptr };
	}

	const VertexMapping::AttributeInfo& VertexMapping::GetAttribute(int attribIndex) const
	{
		BLT_ASSERT(HasAttribute(attribIndex), "No attribute with index {} exists", attribIndex);
		auto it = m_Attributes.find(attribIndex);
		if (it == m_Attributes.end())
		{		
			auto attribInfo = GetMappingInfo(attribIndex);
			const MappingPtr& ptr = attribInfo.first;
			const MappingAttribute& attrib = attribInfo.second;
			byte* dataPtr = (byte*)ptr.Ptr;
			dataPtr += attrib.Offset;
			AttributeInfo info = { (void*)dataPtr, ptr.Stride };
			m_Attributes[attribIndex] = info;
			return m_Attributes[attribIndex];
		}
		return it->second;
	}

	void* VertexMapping::GetAttributePtr(int attributeIndex, int vertexIndex) const
	{
		BLT_ASSERT(HasAttribute(attributeIndex), "Attribute with index {} does not exist", attributeIndex);
		const AttributeInfo& info = GetAttribute(attributeIndex);
		void* ptr = (void*)(((byte*)info.BasePtr) + (intptr_t)((intptr_t)vertexIndex * (intptr_t)info.Stride));
		return ptr;
	}

	void VertexMapping::SetVertexArray(const VertexArray* vertexArray)
	{
		m_Array = vertexArray;
	}

}