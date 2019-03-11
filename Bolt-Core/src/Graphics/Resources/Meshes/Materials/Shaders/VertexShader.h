#pragma once
#include "ShaderProgram.h"

namespace Bolt
{

	class BLT_API VertexShader : public ShaderProgram
	{
	private:
		ShaderStreamPtr m_PositionStream;
		ShaderStreamPtr m_NormalStream;
		ShaderStreamPtr m_TexCoordStream;
		ShaderStreamPtr m_ColorStream;
		ShaderStreamPtr m_TangentStream;

		ShaderAttributePtr m_VertexPosition;

	public:
		VertexShader();

		const ShaderStreamPtr& Position() const;
		const ShaderStreamPtr& Normal() const;
		const ShaderStreamPtr& TexCoord() const;
		const ShaderStreamPtr& Color() const;
		const ShaderStreamPtr& Tangent() const;

		// Set gl_Position, requires vec4
		void SetVertexPosition(ShaderValuePtr value);
		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}