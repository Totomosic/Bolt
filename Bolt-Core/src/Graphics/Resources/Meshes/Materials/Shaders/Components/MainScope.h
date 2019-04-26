#pragma once
#include "ShaderScope.h"

namespace Bolt
{

	class BLT_API MainScope : public ShaderScope
	{
	public:
		MainScope(int index, ShaderScope* parent);

		void Build(ShaderBuilder& builder) const override;
	};

}