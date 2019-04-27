#pragma once
#include "../ShaderOp.h"

namespace Bolt
{

	class BLT_API SubAssignOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		SubAssignOp(const ShaderVariablePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;

	};

}