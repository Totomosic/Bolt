#pragma once
#include "Types.h"
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

		AttributeSetter operator[](int attributeIndex) const;
		AttributeSetter Seek(int attributeIndex) const;

		Vector3f& Position() const;
		Vector3f& Normal() const;
		Vector2f& TexCoord() const;
		Vector4<byte>& Color() const;

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