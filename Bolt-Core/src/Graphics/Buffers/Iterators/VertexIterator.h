#pragma once
#include "bltpch.h"
#include "AttributeSetter.h"
#include "../BufferLayout.h"

namespace Bolt
{

	class BLT_API VertexIterator
	{
	protected:
		void* m_Buffer;
		const BufferLayout* m_Layout;

	public:
		VertexIterator();
		VertexIterator(void* buffer, const BufferLayout* layout);
		virtual ~VertexIterator() {}

		AttributeSetter operator[](int attributeIndex) const;
		AttributeSetter GetSetter(int attributeIndex) const;

		VertexIterator& operator++();
		VertexIterator& operator--();
		VertexIterator operator++(int);
		VertexIterator operator--(int);
		VertexIterator& operator+=(int amount);
		VertexIterator& operator-=(int amount);

		friend VertexIterator operator+(const VertexIterator& left, int right);
		friend VertexIterator operator-(const VertexIterator& left, int right);

		bool operator==(const VertexIterator& other) const;
		bool operator!=(const VertexIterator& other) const;
		bool operator<(const VertexIterator& other) const;
		bool operator<=(const VertexIterator& other) const;
		bool operator>(const VertexIterator& other) const;
		bool operator>=(const VertexIterator& other) const;

		friend class AttributeSetter;
	};

}