#pragma once
#include "ShaderVariable.h"

namespace Bolt
{

	class ShaderPassValue;
	using ShaderPassValuePtr = std::shared_ptr<ShaderPassValue>;

	class BLT_API ShaderPassValue : public ShaderVariable
	{
	public:
		ShaderPassValue(ShaderValuePtr value);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}