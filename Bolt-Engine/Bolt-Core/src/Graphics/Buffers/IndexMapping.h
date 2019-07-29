#pragma once
#include "bltpch.h"
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
			uint32_t MinIndex;
			uint32_t MaxIndex;
			void* Ptr;
		};

	private:
		const IndexArray* m_Array;
		std::vector<MappingPtr> m_MappedPtrs;

	public:
		IndexMapping(const std::vector<MappingPtr>& mappedPtrs);
		IndexMapping(const IndexMapping& other) = delete;
		IndexMapping& operator=(const IndexMapping& other) = delete;
		IndexMapping(IndexMapping&& other);
		IndexMapping& operator=(IndexMapping&& other);
		~IndexMapping();

		int IndexCount() const;

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