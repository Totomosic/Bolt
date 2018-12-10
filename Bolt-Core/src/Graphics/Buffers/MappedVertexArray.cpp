#include "Types.h"
#include "MappedVertexArray.h"
#include "IteratorManager.h"

namespace Bolt
{

	MappedVertexArray::MappedVertexArray()
		: m_MappedPtrs(), m_IteratorManager(nullptr), m_Moved(true)
	{
		
	}

	MappedVertexArray::MappedVertexArray(std::unordered_map<int, MappedVertexArray::MappedPtr> ptrs, IteratorManager* itManager)
		: m_MappedPtrs(std::move(ptrs)), m_IteratorManager(itManager), m_Moved(false)
	{
	
	}

	MappedVertexArray::MappedVertexArray(MappedVertexArray&& other)
		: m_MappedPtrs(std::move(other.m_MappedPtrs)), m_IteratorManager(other.m_IteratorManager), m_Moved(false)
	{
		other.m_Moved = true;
	}

	MappedVertexArray& MappedVertexArray::operator=(MappedVertexArray&& other)
	{
		m_MappedPtrs = std::move(other.m_MappedPtrs);
		m_IteratorManager = other.m_IteratorManager;
		m_Moved = false;
		return *this;
	}

	MappedVertexArray::~MappedVertexArray()
	{
		if (!m_Moved)
		{
			m_IteratorManager->ReleaseIterator();
		}
	}

}