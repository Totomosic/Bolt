#include "Types.h"
#include "IArrayDescriptor.h"

namespace Bolt
{

	IArrayDescriptor::IArrayDescriptor()
		: m_IndexBuffers(), m_LastIndex(0)
	{
	
	}

	int IArrayDescriptor::IndexBufferCount() const
	{
		return m_IndexBuffers.size();
	}

	int IArrayDescriptor::IndexCount() const
	{
		return m_LastIndex;
	}

	IndexMapping IArrayDescriptor::GetMapping() const
	{
		std::vector<IndexMapping::MappingPtr> mappedPtrs;
		for (const IndexBufferInfo& buffer : m_IndexBuffers)
		{
			IndexMapping::MappingPtr ptr;
			ptr.MinIndex = buffer.Range.Min;
			ptr.MaxIndex = buffer.Range.Max;
			ptr.Ptr = buffer.Ptr->Map(Access::ReadWrite);
			mappedPtrs.push_back(ptr);
		}
		return IndexMapping(mappedPtrs);
	}

	void IArrayDescriptor::AddIndexBuffer(IndexBuffer* buffer)
	{
		IndexRange range = { m_LastIndex, m_LastIndex + buffer->IndexCount() };
		m_LastIndex += buffer->IndexCount();
		m_IndexBuffers.push_back({ range, buffer });
	}

	void IArrayDescriptor::UnmapAll() const
	{
		for (const IndexBufferInfo& buffer : m_IndexBuffers)
		{
			buffer.Ptr->Unmap();
		}
	}

}