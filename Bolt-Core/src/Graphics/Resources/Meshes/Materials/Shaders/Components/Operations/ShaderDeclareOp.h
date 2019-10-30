#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API ShaderDeclareOp : public ShaderOp
	{
	private:
		blt::string m_MetaData;
		ShaderVariablePtr m_Variable;

	public:
		ShaderDeclareOp(const ShaderVariablePtr& var, const blt::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(ShaderDeclareOp);

	};

}