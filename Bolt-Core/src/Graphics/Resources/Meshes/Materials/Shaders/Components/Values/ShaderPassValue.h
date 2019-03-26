#pragma once
#include "ShaderVariable.h"

namespace Bolt
{

	class ShaderPassValue;
	using ShaderPassValuePtr = std::shared_ptr<ShaderPassValue>;

	class BLT_API ShaderPassValue : public ShaderVariable
	{
	private:
		ShaderType m_PassTo;

	public:
		ShaderPassValue(ShaderValuePtr value, ShaderType passTo);

		virtual void Build(ShaderBuilder& builder) const override;

		friend class VertexShader;
		friend class FragmentShader;

	};

}