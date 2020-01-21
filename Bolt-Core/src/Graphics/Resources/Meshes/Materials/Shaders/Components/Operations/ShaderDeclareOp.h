#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API ShaderDeclareOp : public ShaderOp
	{
	private:
		std::string m_MetaData;
		ShaderVariablePtr m_Variable;

	public:
		ShaderDeclareOp(const ShaderVariablePtr& var, const std::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(ShaderDeclareOp);

	};

}