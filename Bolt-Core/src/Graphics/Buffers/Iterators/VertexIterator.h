#pragma once
#include "Bolt-Core.h"
#include "AttributeSetter.h"

namespace Bolt
{

	class VertexMapping;

	class BLT_API VertexIterator
	{
	private:
		const VertexMapping* m_Mapping;
		int m_VertexIndex;

	public:
		VertexIterator();
		VertexIterator(const VertexMapping* mapping, int vertexIndex);

		int VertexIndex() const;

		AttributeSetter operator[](int attributeIndex);
		AttributeSetter Seek(int attributeIndex);

		VertexIterator& operator++();
		VertexIterator& operator--();
		VertexIterator& operator++(int);
		VertexIterator& operator--(int);
		VertexIterator& operator+=(int amount);
		VertexIterator& operator-=(int amount);

		bool operator==(const VertexIterator& other) const;
		bool operator!=(const VertexIterator& other) const;
		bool operator<(const VertexIterator& other) const;
		bool operator<=(const VertexIterator& other) const;
		bool operator>(const VertexIterator& other) const;
		bool operator>=(const VertexIterator& other) const;

		friend class AttributeSetter;
	};

}