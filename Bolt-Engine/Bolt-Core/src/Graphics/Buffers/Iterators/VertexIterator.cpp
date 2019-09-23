#include "bltpch.h"
#include "VertexIterator.h"
#include "../VertexMapping.h"

namespace Bolt
{

	VertexIterator::VertexIterator()
		: m_Mapping(nullptr), m_VertexIndex(-1)
	{
	
	}
	
	VertexIterator::VertexIterator(const VertexMapping* mapping, int vertexIndex)
		: m_Mapping(mapping), m_VertexIndex(vertexIndex)
	{
		
	}

	int VertexIterator::VertexIndex() const
	{
		return m_VertexIndex;
	}

	AttributeSetter VertexIterator::operator[](int attributeIndex) const
	{
		return Seek(attributeIndex);
	}

	AttributeSetter VertexIterator::Seek(int attributeIndex) const
	{
		return AttributeSetter(m_Mapping->GetAttributePtr(attributeIndex, m_VertexIndex), attributeIndex);
	}

	Vector3f& VertexIterator::Position() const
	{
		return Seek(BufferLayout::POSITION_INDEX).Read<Vector3f>();
	}

	Vector3f& VertexIterator::Normal() const
	{
		return Seek(BufferLayout::NORMAL_INDEX).Read<Vector3f>();
	}

	Vector2f& VertexIterator::TexCoord() const
	{
		return Seek(BufferLayout::TEXCOORD_INDEX).Read<Vector2f>();
	}

	Vector4<byte>& VertexIterator::Color() const
	{
		return Seek(BufferLayout::COLOR_INDEX).Read<Vector4<byte>>();
	}

	Vector3f& VertexIterator::Tangent() const
	{
		return Seek(BufferLayout::TANGENT_INDEX).Read<Vector3f>();
	}

	VertexIterator& VertexIterator::operator++()
	{
		m_VertexIndex++;
		return *this;
	}

	VertexIterator& VertexIterator::operator--()
	{
		m_VertexIndex--;
		return *this;
	}

	VertexIterator VertexIterator::operator++(int)
	{
		m_VertexIndex++;
		return (*this - 1);
	}

	VertexIterator VertexIterator::operator--(int)
	{
		m_VertexIndex--;
		return (*this + 1);
	}

	VertexIterator& VertexIterator::operator+=(int amount)
	{
		m_VertexIndex += amount;
		return *this;
	}

	VertexIterator& VertexIterator::operator-=(int amount)
	{
		m_VertexIndex -= amount;
		return *this;
	}

	VertexIterator operator+(const VertexIterator& left, int right)
	{
		return VertexIterator(left.m_Mapping, left.m_VertexIndex + right);
	}

	VertexIterator operator-(const VertexIterator& left, int right)
	{
		return (left + (-right));
	}

	bool VertexIterator::operator==(const VertexIterator& other) const
	{
		return (m_VertexIndex == other.m_VertexIndex);
	}

	bool VertexIterator::operator!=(const VertexIterator& other) const
	{
		return !(*this == other);
	}

	bool VertexIterator::operator<(const VertexIterator& other) const
	{
		return (m_VertexIndex < other.m_VertexIndex);
	}

	bool VertexIterator::operator<=(const VertexIterator& other) const
	{
		return (m_VertexIndex <= other.m_VertexIndex);
	}

	bool VertexIterator::operator>(const VertexIterator& other) const
	{
		return (m_VertexIndex > other.m_VertexIndex);
	}

	bool VertexIterator::operator>=(const VertexIterator& other) const
	{
		return (m_VertexIndex >= other.m_VertexIndex);
	}

}