#pragma once
#include "ShaderOp.h"
#include "../Values/ShaderLiteral.h"

namespace Bolt
{

	class BLT_API DeclareArrayOp : public ShaderOp
	{
	private:
		blt::string m_MetaData;
		ShaderVariablePtr m_Variable;
		ShaderLiteralPtr m_Length;

	public:
		DeclareArrayOp(const ShaderVariablePtr& arr, const ShaderLiteralPtr& length, const blt::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(DeclareArrayOp);

	};

}