#pragma once
#include "ShaderScope.h"

namespace Bolt
{

	class BLT_API ForLoopScope : public ShaderScope
	{
	private:
		std::unique_ptr<ShaderOp> m_InitOp;
		ShaderValuePtr m_Condition;
		std::unique_ptr<ShaderOp> m_IterationOp;

	public:
		ForLoopScope(int scopeIndex, std::unique_ptr<ShaderOp>&& init, const ShaderValuePtr& condition, std::unique_ptr<ShaderOp>&& iteration);

		void Build(ShaderBuilder& builder) const override;
		std::unique_ptr<ShaderScope> Clone() const override;

	};

}