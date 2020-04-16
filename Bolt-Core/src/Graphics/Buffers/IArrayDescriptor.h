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
		uint32_t operator()(const Bolt::IndexRange& range) const
		{
			return (uint32_t)range.Min;
		}
	};

}

namespace Bolt
{

	class BLT_API IArrayDescriptor
	{
	public:
		struct BLT_API IndexBufferInfo
		{
		public:
			IndexRange Range;
			IndexBuffer* Ptr;
		};

	private:
		std::vector<IndexBufferInfo> m_IndexBuffers;
		int m_LastIndex;

	public:
		IArrayDescriptor();

		int IndexBufferCount() const;
		int IndexCount() const;

		friend class IndexArray;
		
	private:
		void AddIndexBuffer(IndexBuffer* buffer);

	};

}