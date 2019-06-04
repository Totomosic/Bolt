#pragma once
#include "ShaderOp.h"

namespace Bolt
{

	class BLT_API DiscardOp : public ShaderOp
	{
	public:
		DiscardOp();

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADEROP_CLONE(DiscardOp);

	};

}