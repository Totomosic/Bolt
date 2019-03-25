#pragma once
#include "ShaderProgram.h"

namespace Bolt
{

	class BLT_API FragmentShader : public ShaderProgram
	{
	private:
		FragColorAttributePtr m_FragColor;

	public:
		FragmentShader();

		// Set FragColor, requires vec4
		void SetFragColor(ShaderValuePtr value);
		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}