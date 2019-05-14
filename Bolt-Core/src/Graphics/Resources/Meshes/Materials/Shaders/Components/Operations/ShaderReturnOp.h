#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API ShaderReturnOp : public ShaderOp
	{
	private:
		ShaderValuePtr m_Value;

	public:
		ShaderReturnOp(const ShaderValuePtr& value = nullptr);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(ShaderReturnOp);

	};

}