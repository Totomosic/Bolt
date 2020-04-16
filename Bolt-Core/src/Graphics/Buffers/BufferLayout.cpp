#include "bltpch.h"
#include "BufferLayout.h"

namespace Bolt
{

	BufferLayout::BufferLayout()
		: m_Attributes(), m_Stride(0)
	{
	
	}

	uint32_t BufferLayout::Stride() const
	{
		return m_Stride;
	}

	uint32_t BufferLayout::Size() const
	{
		return Stride();
	}

	int BufferLayout::OffsetOf(int index) const
	{
		return m_Attributes.at(index).Offset;
	}

	bool BufferLayout::HasAttribute(int index) const
	{
		return index < m_Attributes.size();
	}

	int BufferLayout::AttributeCount() const
	{
		return m_Attributes.size();
	}
	
	const BufferLayout::VertexAttribute& BufferLayout::GetAttribute(int index) const
	{
		return m_Attributes.at(index);
	}

	const std::vector<BufferLayout::VertexAttribute>& BufferLayout::GetAttributes() const
	{
		return m_Attributes;
	}

	void BufferLayout::AddAttribute(int count, DataType type, bool normalized)
	{
		VertexAttribute attrib;
		attrib.Index = m_Attributes.size();
		attrib.Count = count;
		attrib.Type = type;
		attrib.Normalised = normalized;
		attrib.Offset = m_Stride;
		AddAttribute(attrib);
	}

	bool BufferLayout::operator==(const BufferLayout& other) const
	{
		return !(*this != other);
	}

	bool BufferLayout::operator!=(const BufferLayout& other) const
	{
		// Not a perfect test
		if (AttributeCount() != other.AttributeCount())
		{
			return true;
		}
		return Stride() != other.Stride();
	}

	void BufferLayout::AddAttribute(const VertexAttribute& attribute)
	{
		m_Attributes.push_back(attribute);
		int dataTypeSize = (attribute.Type == DataType::UByte) ? sizeof(byte) : sizeof(float);
		m_Stride += attribute.Count * dataTypeSize;
	}

	BufferLayout BufferLayout::Default()
	{
		BufferLayout layout;
		layout.AddAttribute<Vector3f>(1);
		layout.AddAttribute<Vector3f>(1);
		layout.AddAttribute<Vector2f>(1);
		layout.AddAttribute<byte>(4, true);
		layout.AddAttribute<Vector3f>(1);
		return layout;
	}

}