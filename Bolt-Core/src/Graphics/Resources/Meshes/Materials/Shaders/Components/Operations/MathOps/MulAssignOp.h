#pragma once
#include "../ShaderOp.h"

namespace Bolt
{

	class BLT_API MulAssignOp : public ShaderOp
	{
	private:
		ShaderLValuePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		MulAssignOp(const ShaderLValuePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(MulAssignOp);

	};

}