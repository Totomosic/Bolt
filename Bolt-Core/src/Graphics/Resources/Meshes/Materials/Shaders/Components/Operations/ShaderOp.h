#pragma once
#include "../Values/ShaderVariable.h"

namespace Bolt
{

	class ShaderBuilder;

	class BLT_API ShaderOp
	{
	public:
		virtual void Build(ShaderBuilder& builder) const = 0;
	};

}