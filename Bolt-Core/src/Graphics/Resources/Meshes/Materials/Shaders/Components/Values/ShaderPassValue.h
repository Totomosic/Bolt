#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderPassValue;
	using ShaderPassValuePtr = std::shared_ptr<ShaderPassValue>;

	class BLT_API ShaderPassValue : public ShaderValue
	{
	public:
		ShaderPassValue(ShaderValuePtr value);

	};

}