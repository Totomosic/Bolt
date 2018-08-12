#include "AttributeSetter.h"
#include "VertexIterator.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"

namespace Bolt
{

	AttributeSetter::AttributeSetter(byte* ptr, int attributeIndex)
		: m_Ptr(ptr), m_AttribIndex(attributeIndex)
	{
	
	}

	AttributeSetter::AttributeSetter(AttributeSetter&& other)
		: m_Ptr(other.m_Ptr), m_AttribIndex(other.m_AttribIndex)
	{
		other.m_Ptr = nullptr;
	}

	AttributeSetter& AttributeSetter::operator=(AttributeSetter&& other)
	{
		m_Ptr = other.m_Ptr;
		m_AttribIndex = other.m_AttribIndex;
		other.m_Ptr = nullptr;
		return *this;
	}

	AttributeSetter::~AttributeSetter()
	{
		
	}

	const byte* AttributeSetter::Get() const
	{
		return m_Ptr;
	}

	byte* AttributeSetter::Get()
	{
		return m_Ptr;
	}

	int AttributeSetter::AttributeIndex() const
	{
		return m_AttribIndex;
	}

	bool AttributeSetter::ValidateAttribute(int attribIndex, DataType type, int count) const
	{
		return true;
	}

}