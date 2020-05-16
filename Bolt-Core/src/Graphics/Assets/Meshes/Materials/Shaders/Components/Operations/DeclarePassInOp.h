#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API DeclarePassInOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_InVar;
		ShaderVariablePtr m_OutVar;
		std::string m_MetaData;

	public:
		DeclarePassInOp(const ShaderVariablePtr& inVar, const ShaderVariablePtr& outVar, const std::string& meta = "");

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(DeclarePassInOp);

	};

}