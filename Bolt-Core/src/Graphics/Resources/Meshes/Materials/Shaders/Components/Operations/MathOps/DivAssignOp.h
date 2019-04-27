#pragma once
#include "../ShaderOp.h"

namespace Bolt
{

	class BLT_API DivAssignOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		DivAssignOp(const ShaderVariablePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;

	};

}