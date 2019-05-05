#pragma once
#include "../Values/ShaderVariable.h"

namespace Bolt
{

#define BLT_DEFAULT_SHADEROP_CLONE(OpType) inline virtual std::unique_ptr<ShaderOp> Clone() const override { return std::make_unique<OpType>(*this); }

	class ShaderBuilder;

	class BLT_API ShaderOp
	{
	public:
		virtual void Build(ShaderBuilder& builder) const = 0;
		virtual std::unique_ptr<ShaderOp> Clone() const = 0;
	};

}