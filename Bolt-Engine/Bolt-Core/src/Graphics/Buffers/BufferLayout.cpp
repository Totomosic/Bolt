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
		return m_Attributes.find(index) != m_Attributes.end();
	}

	int BufferLayout::AttributeCount() const
	{
		return m_Attributes.size();
	}
	
	const BufferLayout::VertexAttribute& BufferLayout::GetAttribute(int index) const
	{
		return m_Attributes.at(index);
	}

	std::vector<BufferLayout::VertexAttribute> BufferLayout::GetAttributes() const
	{
		std::vector<VertexAttribute> result;
		for (const auto& pair : m_Attributes)
		{
			result.push_back(pair.second);
		}
		return result;
	}

	void BufferLayout::AddAttribute(int count, DataType type, bool normalized)
	{
		AddAttribute(m_Attributes.size(), count, type, normalized);
	}

	void BufferLayout::AddAttribute(int index, int count, DataType type, bool normalized)
	{
		VertexAttribute attrib;
		attrib.Index = index;
		attrib.Count = count;
		attrib.Type = type;
		attrib.Normalised = normalized;
		attrib.Offset = m_Stride;
		AddAttribute(attrib);
	}

	void BufferLayout::AddAttribute(const VertexAttribute& attribute)
	{
		m_Attributes[attribute.Index] = attribute;
		int dataTypeSize = (attribute.Type == DataType::UByte) ? sizeof(byte) : sizeof(float);
		m_Stride += attribute.Count * dataTypeSize;
	}

	const std::unordered_map<int, BufferLayout::VertexAttribute>& BufferLayout::GetAttributesMap() const
	{
		return m_Attributes;
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