#include "Types.h"

#include "AttributeSetter.h"
#include "VertexIterator.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"

namespace Bolt
{

	AttributeSetter::AttributeSetter(void* ptr, int attributeIndex)
		: m_Ptr(ptr), m_AttribIndex(attributeIndex)
	{
	
	}

	const void* AttributeSetter::Get() const
	{
		return m_Ptr;
	}

	void* AttributeSetter::Get()
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