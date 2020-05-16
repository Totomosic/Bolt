#include "bltpch.h"
#include "ShaderFactory.h"

namespace Bolt
{

	ShaderFactory::ShaderFactory()
		: m_Vertex(), m_Fragment(), m_CurrentShaderInstance(nullptr), m_IsValid(false)
	{
	
	}

	VertexShader& ShaderFactory::Vertex()
	{
		m_IsValid = false;
		return m_Vertex;
	}

	FragmentShader& ShaderFactory::Fragment()
	{
		m_IsValid = false;
		return m_Fragment;
	}

	void ShaderFactory::ForceRecompile()
	{
		m_IsValid = false;
	}

	ShaderLinkContext ShaderFactory::BuildShader() const
	{
		if (!m_IsValid)
		{
			CompiledShaderProgram vertexSource = m_Vertex.Compile();
			CompiledShaderProgram fragmentSource = m_Fragment.Compile();
			m_CurrentShaderInstance = std::make_shared<ShaderInstance>(vertexSource, fragmentSource);
			m_IsValid = true;
		}
		return ShaderLinkContext(m_CurrentShaderInstance);
	}

}