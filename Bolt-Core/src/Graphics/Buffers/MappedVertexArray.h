#pragma once
#include "BufferLayout.h"

namespace Bolt
{

	class IteratorManager;

	struct BLT_API MappedVertexArray
	{
	public:
		struct BLT_API MappedPtr
		{
		public:
			byte* Ptr;
			const BufferLayout* Layout;
		};

	public:
		std::unordered_map<int, MappedPtr> m_MappedPtrs;
		IteratorManager* m_IteratorManager;
		bool m_Moved;

	public:
		MappedVertexArray();
		MappedVertexArray(std::unordered_map<int, MappedPtr> ptrs, IteratorManager* itManager);
		MappedVertexArray(const MappedVertexArray& other) = delete;
		MappedVertexArray(MappedVertexArray&& other);
		MappedVertexArray& operator=(const MappedVertexArray& other) = delete;
		MappedVertexArray& operator=(MappedVertexArray&& other);
		~MappedVertexArray();

	};

}