#pragma once
#include "Graphics/Buffers/Iterators/VertexIterator.h"

namespace Bolt
{

	template<size_t VCount>
	struct BLT_API Face
	{
	public:
		VertexIterator Vertices[VCount];
	};

	typedef Face<2> Line;
	typedef Face<3> Triangle;
	typedef Face<4> Quad;

}