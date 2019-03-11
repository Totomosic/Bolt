#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderUniform;
	using ShaderUniformPtr = std::shared_ptr<ShaderUniform>;

	class BLT_API ShaderUniform : public ShaderValue
	{
	protected:
		ShaderValuePtr m_DefaultValue;

	public:
		ShaderUniform(ValueType type);
		ShaderUniform(ShaderValuePtr defaultValue);

	};

}