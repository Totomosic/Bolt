#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API ShaderDefineOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_Variable;

	public:
		ShaderDefineOp(const ShaderVariablePtr& var);

		void Build(ShaderBuilder& builder) const override;

	};

}