#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API SetValueOp : public ShaderOp
	{
	private:
		ShaderLValuePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		SetValueOp(const ShaderLValuePtr& var, const ShaderValuePtr& value);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(SetValueOp);

	};

}