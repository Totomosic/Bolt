#pragma once
#include "ShaderScope.h"

namespace Bolt
{

	class BLT_API IfScope : public ShaderScope
	{
	private:
		ShaderValuePtr m_Condition;

	public:
		IfScope(int scopeIndex, const ShaderValuePtr& condition);

		void Build(ShaderBuilder& builder) const override;
		std::unique_ptr<ShaderScope> Clone() const override;

	};

}