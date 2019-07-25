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

		// Discards current fragment (only valid in fragment shader)
		void Discard();

		void SetFragColor(const ShaderValuePtr& value);
		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}