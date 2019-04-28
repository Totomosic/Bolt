#pragma once
#include "../ShaderOp.h"

namespace Bolt
{

	class BLT_API DivAssignOp : public ShaderOp
	{
	private:
		ShaderLValuePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		DivAssignOp(const ShaderLValuePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;

	};

}