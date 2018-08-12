#pragma once
#include "IndexBuffer.h"

namespace Bolt
{

	struct BLT_API IndexRange
	{
	public:
		int Min;
		int Max;

	public:
		inline bool Within(int index) const { return (index >= Min && index < Max); }

	};

}

namespace std
{

	template<>
	struct BLT_API hash<Bolt::IndexRange>
	{
		size_t operator()(const Bolt::IndexRange& range) const
		{
			return (size_t)range.Min;
		}
	};

}

namespace Bolt
{

	class BLT_API IArrayDescriptor
	{
	public:
		struct BLT_API QueryResult
		{
			IndexBuffer* Buffer;
			int IndexOffset;
		};

	private:
		std::vector<IndexBuffer*> m_IndexBuffers;
		std::vector<IndexRange> m_Ranges;
		int m_LastIndex;

	public:
		IArrayDescriptor();
		IArrayDescriptor(const IArrayDescriptor& other) = default;
		IArrayDescriptor& operator=(const IArrayDescriptor& other) = default;
		IArrayDescriptor(IArrayDescriptor&& other) = default;
		IArrayDescriptor& operator=(IArrayDescriptor&& other) = default;
		~IArrayDescriptor() = default;

		int IndexBufferCount() const;
		int IndexCount() const;
		QueryResult QueryIndex(int requestedIndex) const;
		IndexBuffer* GetIndexBuffer(int requestedIndex) const;
		int CalculateBufferOffset(int index) const;

		friend class IndexArray;
		
	private:
		void AddIndexBuffer(IndexBuffer* buffer);

	};

}