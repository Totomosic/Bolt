#include "bltpch.h"
#include "InputPort.h"

namespace Bolt
{

	InputPort::InputPort() : InputPort(ValueType::Void)
	{
		
	}

	InputPort::InputPort(ValueType type)
		: m_Type(type)
	{
	
	}

	bool InputPort::HasType() const
	{
		return m_Type != ValueType::Void;
	}

	ValueType InputPort::GetType() const
	{
		return m_Type;
	}

}