#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class ShaderScope;

	class BLT_API CreateScopeOp : public ShaderOp
	{
	private:
		std::unique_ptr<ShaderScope> m_Scope;

	public:
		CreateScopeOp(std::unique_ptr<ShaderScope>&& scope);

		void Build(ShaderBuilder& builder) const override;
		virtual std::unique_ptr<ShaderOp> Clone() const override;

	};

}