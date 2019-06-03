#include "bltpch.h"
#include "OutputPort.h"

namespace Bolt
{

	OutputPort::OutputPort() : OutputPort(ValueType::Void)
	{
	
	}

	OutputPort::OutputPort(ValueType type)
		: m_Type(type), m_IsUsed(false)
	{
	
	}

	bool OutputPort::HasType() const
	{
		return m_Type != ValueType::Void;
	}

	ValueType OutputPort::GetType() const
	{
		return m_Type;
	}

	bool OutputPort::IsUsed() const
	{
		return m_IsUsed;
	}

	void OutputPort::SetIsUsed(bool isUsed)
	{
		m_IsUsed = isUsed;
	}

}