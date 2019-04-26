#pragma once
#include "ShaderScope.h"

namespace Bolt
{

	class BLT_API GlobalScope : public ShaderScope
	{
	public:
		GlobalScope();

		void Build(ShaderBuilder& builder) const override;
	};

}