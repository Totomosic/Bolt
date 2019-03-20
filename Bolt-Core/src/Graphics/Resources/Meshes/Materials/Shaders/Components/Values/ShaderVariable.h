#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderVariable;
	using ShaderVariablePtr = std::shared_ptr<ShaderVariable>;

	class BLT_API ShaderVariable : public ShaderValue
	{
	protected:
		mutable blt::string m_Name;
		ShaderValuePtr m_Value;

	public:
		ShaderVariable(ValueType type);
		ShaderVariable(ShaderValuePtr value);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}