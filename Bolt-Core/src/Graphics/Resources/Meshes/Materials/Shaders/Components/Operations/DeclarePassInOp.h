#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API DeclarePassInOp : public ShaderOp
	{
	private:
		ShaderVariablePtr m_InVar;
		ShaderVariablePtr m_OutVar;
		blt::string m_MetaData;

	public:
		DeclarePassInOp(const ShaderVariablePtr& inVar, const ShaderVariablePtr& outVar, const blt::string& meta = "");

		void Build(ShaderBuilder& builder) const override;

	};

}