#pragma once
#include "ShaderProgram.h"

namespace Bolt
{

	class BLT_API FragmentShader : public ShaderProgram
	{
	private:
		ShaderVariablePtr m_FragColor;

	public:
		FragmentShader();

		const ShaderVariablePtr& FragColor() const;

		void SetFragColor(const ShaderValuePtr& value);
		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}