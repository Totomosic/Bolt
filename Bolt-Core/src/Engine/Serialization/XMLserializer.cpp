#include "bltpch.h"
#include "XMLserializer.h"

namespace Bolt
{

	const blt::string XMLserializer::POINTER_VALUES_CHILD = "pointer_values";

	XMLserializer::XMLserializer()
		: m_OutputNode(nullptr), m_InputNode(nullptr), m_SerializedValues(), m_SerializedCount(0)
	{
	
	}

	XMLserializer::XMLserializer(XMLnode& xmlNode, bool writing)
		: m_OutputNode(nullptr), m_InputNode(nullptr), m_SerializedValues(), m_SerializedCount(0)
	{
		if (writing)
		{
			m_OutputNode = &xmlNode;
		}
		else
		{
			m_InputNode = &xmlNode;
		}
	}

	bool XMLserializer::IsWriting() const
	{
		return m_OutputNode != nullptr;
	}
	
	XMLnode* XMLserializer::OutputNode() const
	{
		return m_OutputNode;
	}

	XMLnode* XMLserializer::InputNode() const
	{
		return m_InputNode;
	}

	void XMLserializer::SetOutputNode(XMLnode& node)
	{
		m_OutputNode = &node;
		m_InputNode = nullptr;
	}

	void XMLserializer::SetInputNode(XMLnode& node)
	{
		m_OutputNode = &node;
		m_InputNode = nullptr;
	}

	id_t XMLserializer::AddSerializedValue(void* value)
	{
		id_t id = m_SerializedCount;
		m_SerializedValues[value] = id;
		m_SerializedCount++;
		return id;
	}

	bool XMLserializer::HasSerializedValue(void* value) const
	{
		return m_SerializedValues.find(value) != m_SerializedValues.end();
	}

	id_t XMLserializer::IdOfSerializedValue(void* value) const
	{
		return m_SerializedValues.at(value);
	}

	void XMLserializer::ClearSerializedValues()
	{
		m_SerializedValues.clear();
		m_SerializedCount = 0;
	}

}