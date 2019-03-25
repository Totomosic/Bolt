#include "Types.h"
#include "ShaderRendererUniform.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderRendererUniform::ShaderRendererUniform(RendererUniform uniform) : ShaderUniform(GetTypeOfRendererUniform(uniform)),
		m_Uniform(uniform)
	{
	
	}

	RendererUniform ShaderRendererUniform::UniformType() const
	{
		return m_Uniform;
	}

}