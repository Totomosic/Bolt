#pragma once
#include "Types.h"
#include "Iterators/IndexIterator.h"

namespace Bolt
{

	class IndexArray;

	class BLT_API IndexMapping
	{
	public:
		struct BLT_API MappingPtr
		{
		public:
			uint MinIndex;
			uint MaxIndex;
			void* Ptr;
		};

	private:
		const IndexArray* m_Array;
		std::vector<MappingPtr> m_MappedPtrs;

	public:
		IndexMapping(const std::vector<MappingPtr>& mappedPtrs);
		~IndexMapping();

		IndexIterator Begin() const;
		IndexIterator End() const;
		IndexIterator GetIndex(int index) const;

		friend class IndexIterator;
		friend class IndexArray;

	private:
		void* GetIndexPtr(int index) const;
		void SetIndexArray(const IndexArray* indexArray);

	};

}