#include "Types.h"
#include "ShaderFactory.h"

namespace Bolt
{

	ShaderFactory::ShaderFactory()
		: m_Vertex(), m_Fragment()
	{
	
	}

	VertexShader& ShaderFactory::Vertex()
	{
		return m_Vertex;
	}

	FragmentShader& ShaderFactory::Fragment()
	{
		return m_Fragment;
	}

	ShaderLinkContext ShaderFactory::BuildShader() const
	{
		CompiledShaderProgram vertexSource = m_Vertex.Compile();
		CompiledShaderProgram fragmentSource = m_Fragment.Compile();
		ShaderInstance instance(vertexSource, fragmentSource);
		return ShaderLinkContext(std::move(instance));
	}

}