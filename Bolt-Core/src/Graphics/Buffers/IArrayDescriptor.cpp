#include "bltpch.h"

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

	void IArrayDescriptor::AddIndexBuffer(IndexBuffer* buffer)
	{
		IndexRange range = { m_LastIndex, m_LastIndex + buffer->IndexCount() };
		m_LastIndex += buffer->IndexCount();
		m_IndexBuffers.push_back({ range, buffer });
	}

}