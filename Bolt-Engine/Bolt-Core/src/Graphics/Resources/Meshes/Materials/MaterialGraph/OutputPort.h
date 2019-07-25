#pragma once
#include "MaterialGraphEnums.h"

namespace Bolt
{

	class BLT_API OutputPort
	{
	private:
		// use ValueType::Void for multiple types
		ValueType m_Type;
		bool m_IsUsed;

	public:
		OutputPort();
		OutputPort(ValueType type);

		// Returns false if type is ValueType::Void
		bool HasType() const;
		ValueType GetType() const;
		bool IsUsed() const;

		void SetIsUsed(bool isUsed);

	};

}