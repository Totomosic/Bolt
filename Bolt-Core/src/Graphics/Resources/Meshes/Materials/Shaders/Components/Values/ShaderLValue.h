#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderLValue;
	using ShaderLValuePtr = std::shared_ptr<ShaderLValue>;

	class BLT_API ShaderLValue : public ShaderValue
	{
	protected:
		mutable blt::string m_Name;

	public:
		ShaderLValue();
		ShaderLValue(ValueType type, ValueTypeDim dim);

	};

}