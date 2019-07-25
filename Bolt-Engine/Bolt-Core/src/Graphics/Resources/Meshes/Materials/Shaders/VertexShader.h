#pragma once
#include "ShaderProgram.h"

namespace Bolt
{

	class BLT_API VertexShader : public ShaderProgram
	{
	private:
		ShaderVariablePtr m_PositionStream;
		ShaderVariablePtr m_NormalStream;
		ShaderVariablePtr m_TexCoordStream;
		ShaderVariablePtr m_ColorStream;
		ShaderVariablePtr m_TangentStream;

		ShaderVariablePtr m_VertexPosition;

	public:
		VertexShader();

		const ShaderVariablePtr& Position() const;
		const ShaderVariablePtr& Normal() const;
		const ShaderVariablePtr& TexCoord() const;
		const ShaderVariablePtr& Color() const;
		const ShaderVariablePtr& Tangent() const;
		const ShaderVariablePtr& VertexPosition() const;
		const ShaderVariablePtr& Stream(ShaderStream stream) const;

		void SetVertexPosition(const ShaderValuePtr& value);

		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}