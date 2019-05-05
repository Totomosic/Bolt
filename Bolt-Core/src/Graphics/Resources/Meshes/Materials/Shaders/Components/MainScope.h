#pragma once
#include "ShaderScope.h"

namespace Bolt
{

	class BLT_API MainScope : public ShaderScope
	{
	public:
		MainScope(int index);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADERSCOPE_CLONE(MainScope);
	};

}