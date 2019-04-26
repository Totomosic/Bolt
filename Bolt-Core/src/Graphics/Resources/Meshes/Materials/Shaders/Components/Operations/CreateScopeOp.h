#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class ShaderScope;

	class BLT_API CreateScopeOp : public ShaderOp
	{
	private:
		ShaderScope* m_Scope;

	public:
		CreateScopeOp(ShaderScope* scope);

		void Build(ShaderBuilder& builder) const override;

	};

}