#include "IArrayDescriptor.h"

namespace Bolt
{

	IArrayDescriptor::IArrayDescriptor()
		: m_IndexBuffers(), m_Ranges(), m_LastIndex(0)
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

	IArrayDescriptor::QueryResult IArrayDescriptor::QueryIndex(int requestedIndex) const
	{		
		int currentSize = 0;
		for (int i = 0; i < m_IndexBuffers.size(); i++)
		{
			if (m_Ranges[i].Within(requestedIndex))
			{
				return { m_IndexBuffers[i], requestedIndex - currentSize };
			}
			currentSize += m_IndexBuffers[i]->IndexCount();
		}
		return { nullptr, 0 };
	}

	IndexBuffer* IArrayDescriptor::GetIndexBuffer(int requestedIndex) const
	{
		return QueryIndex(requestedIndex).Buffer;
	}

	int IArrayDescriptor::CalculateBufferOffset(int requestedIndex) const
	{
		return QueryIndex(requestedIndex).IndexOffset;
	}

	void IArrayDescriptor::AddIndexBuffer(IndexBuffer* buffer)
	{
		IndexRange range = { m_LastIndex, m_LastIndex + buffer->IndexCount() };
		m_LastIndex += buffer->IndexCount();
		m_IndexBuffers.push_back(buffer);
		m_Ranges.push_back(range);
	}

}