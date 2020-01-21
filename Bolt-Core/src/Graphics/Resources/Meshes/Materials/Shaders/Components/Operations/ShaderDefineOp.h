#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API ShaderDefineOp : public ShaderOp
	{
	private:
		std::string m_MetaData;
		ShaderVariablePtr m_Variable;
		ShaderValuePtr m_Value;

	public:
		ShaderDefineOp(const ShaderVariablePtr& var, const ShaderValuePtr& value, const std::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(ShaderDefineOp);

	};

}