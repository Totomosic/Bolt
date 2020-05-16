#pragma once
#include "Graphics/Buffers/Iterators/VertexIterator.h"

namespace Bolt
{

	template<uint32_t VCount, typename IteratorT = VertexIterator>
	struct BLT_API Face
	{
	public:
		IteratorT Vertices[VCount];
	};

	template<typename IteratorT = VertexIterator>
	using Triangle = Face<3, IteratorT>;

	template<typename IteratorT = VertexIterator>
	using Quad = Face<4, IteratorT>;

}