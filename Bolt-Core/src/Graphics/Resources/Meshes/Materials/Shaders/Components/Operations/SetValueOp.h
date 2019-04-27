#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API SetValueOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		SetValueOp(const ShaderVariablePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;

	};

}