#include "Types.h"
#include "IndexMapping.h"
#include "IndexArray.h"

namespace Bolt
{

	IndexMapping::IndexMapping(const std::vector<IndexMapping::MappingPtr>& mappedPtrs)
		: m_Array(nullptr), m_MappedPtrs(mappedPtrs)
	{
	
	}

	IndexMapping::IndexMapping(IndexMapping&& other)
		: m_Array(other.m_Array), m_MappedPtrs(std::move(other.m_MappedPtrs))
	{
		other.m_Array = nullptr;
	}

	IndexMapping& IndexMapping::operator=(IndexMapping&& other)
	{
		const IndexArray* myArray = m_Array;
		m_Array = other.m_Array;
		m_MappedPtrs = std::move(m_MappedPtrs);
		other.m_Array = myArray;
		return *this;
	}

	IndexMapping::~IndexMapping()
	{
		if (m_Array != nullptr)
		{
			m_Array->SetMapped(false);
		}
	}

	IndexIterator IndexMapping::Begin() const
	{
		return GetIndex(0);
	}

	IndexIterator IndexMapping::End() const
	{
		return GetIndex(m_Array->IndexCount());
	}
	IndexIterator IndexMapping::GetIndex(int index) const
	{
		return IndexIterator(this, index);
	}

	void* IndexMapping::GetIndexPtr(int index) const
	{
		for (const MappingPtr& ptr : m_MappedPtrs)
		{
			if (index >= ptr.MinIndex && index <= ptr.MaxIndex)
			{
				return (void*)((byte*)ptr.Ptr + index * sizeof(uint));
			}
		}
		return nullptr;
	}

	void IndexMapping::SetIndexArray(const IndexArray* indexArray)
	{
		m_Array = indexArray;
	}

}