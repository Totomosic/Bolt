#pragma once
#include "ShaderVariable.h"
#include "ShaderLiteral.h"

namespace Bolt
{

	class ShaderUniform;
	using ShaderUniformPtr = std::shared_ptr<ShaderUniform>;

	class BLT_API ShaderUniform : public ShaderVariable
	{
	protected:
		ShaderLiteralPtr m_DefaultValue;

	public:
		ShaderUniform(ValueType type);
		ShaderUniform(ShaderLiteralPtr defaultValue);

		virtual void Build(ShaderBuilder& builder) const override;

	};

}