#include "Types.h"
#include "IteratorManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Bolt
{

	IteratorManager::IteratorManager(VertexArray* vArray)
		: m_Array(vArray), m_IteratorCount(0)
	{
	
	}

	bool IteratorManager::IsMapped() const
	{
		return m_IteratorCount > 0;
	}

	VertexIterator IteratorManager::CreateNewIterator(int vertexIndex) const
	{
		m_IteratorCount++;
		std::unordered_map<int, MappedVertexArray::MappedPtr> mappedPtrs;
		for (int i = 0; i < m_Array->Descriptor().AttributeCount(); i++)
		{
			VertexBuffer* buffer = m_Array->Descriptor().GetAttribute(i);
			mappedPtrs[i] = { (byte*)buffer->Map(Access::ReadWrite), &buffer->Layout() };
		}
		MappedVertexArray result(std::move(mappedPtrs), (IteratorManager*)this);
		return VertexIterator(std::move(result), vertexIndex);
	}

	void IteratorManager::ReleaseIterator() const
	{
		m_IteratorCount--;
		if (m_IteratorCount <= 0)
		{
			for (int i = 0; i < m_Array->Descriptor().AttributeCount(); i++)
			{
				VertexBuffer* buffer = m_Array->Descriptor().GetAttribute(i);
				buffer->Unmap();
			}
		}
	}

}