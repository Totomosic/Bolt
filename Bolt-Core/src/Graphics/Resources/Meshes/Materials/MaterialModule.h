#pragma once
#include "Shaders/__Shaders__.h"

namespace Bolt
{

	class BLT_API MaterialModule
	{
	protected:
		ShaderType m_Shader;

	public:
		MaterialModule(ShaderType shader);

		ShaderType GetShader() const;
		virtual void AddInstructions(ShaderFactory& factory) = 0;

	protected:
		ShaderValuePtr GetValue(ShaderFactory& factory, ShaderValuePtr value) const;

	};

}