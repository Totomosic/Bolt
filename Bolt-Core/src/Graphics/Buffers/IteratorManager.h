#pragma once
#include "Iterators\VertexIterator.h"

namespace Bolt
{

	class VertexArray;

	class BLT_API IteratorManager
	{
	private:
		VertexArray* m_Array;
		mutable int m_IteratorCount;

	public:
		IteratorManager(VertexArray* vArray);

		bool IsMapped() const;

		VertexIterator CreateNewIterator(int vertexIndex) const;
		void ReleaseIterator() const;

	};

}