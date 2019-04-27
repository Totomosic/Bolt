#pragma once
#include "../ShaderOp.h"

namespace Bolt
{

	class BLT_API AddAssignOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		AddAssignOp(const ShaderVariablePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;

	};

}