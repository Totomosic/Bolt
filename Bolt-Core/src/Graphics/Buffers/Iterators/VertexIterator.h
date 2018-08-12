#pragma once
#include "Bolt-Core.h"
#include "..\BufferLayout.h"
#include "AttributeSetter.h"
#include "..\MappedVertexArray.h"

namespace Bolt
{

	class BLT_API VertexIterator
	{
	private:
		MappedVertexArray m_VertexBuffers;
		int m_VertexIndex;

	public:
		VertexIterator();
		VertexIterator(MappedVertexArray&& vertexBuffers, int vertexIndex);
		~VertexIterator() noexcept;

		VertexIterator(const VertexIterator& other) = delete;
		VertexIterator& operator=(const VertexIterator& other) = delete;
		VertexIterator(VertexIterator&& other);
		VertexIterator& operator=(VertexIterator&& other);

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
		friend class IteratorManager;

	};

}