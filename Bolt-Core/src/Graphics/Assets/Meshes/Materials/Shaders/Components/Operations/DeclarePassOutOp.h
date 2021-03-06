#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API DeclarePassOutOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_OutVar;
		std::string m_MetaData;

	public:
		DeclarePassOutOp(const ShaderVariablePtr& outVar, const std::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(DeclarePassOutOp);

	};

}