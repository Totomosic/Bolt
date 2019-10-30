#include "bltpch.h"
#include "DiscardOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	DiscardOp::DiscardOp() : ShaderOp()
	{
		
	}

	void DiscardOp::Build(ShaderBuilder& builder) const
	{
		builder.Write("discard");
	}

}