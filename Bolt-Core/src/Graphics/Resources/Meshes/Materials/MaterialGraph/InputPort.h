#pragma once
#include "MaterialGraphEnums.h"

namespace Bolt
{

	class BLT_API InputPort
	{
	private:
		// Use ValueType::Void if multiple types
		ValueType m_Type;

	public:
		InputPort();
		InputPort(ValueType type);

		// Returns false if type is ValueType::Void
		bool HasType() const;
		ValueType GetType() const;

	};

}