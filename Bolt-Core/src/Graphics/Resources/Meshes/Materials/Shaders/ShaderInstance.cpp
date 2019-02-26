#include "Types.h"
#include "ShaderInstance.h"
#include "ShaderProgramFactory.h"

namespace Bolt
{

	ShaderInstance::ShaderInstance(const blt::string& vertexSource, const blt::string& fragmentSource, const std::vector<RendererUniformInfo>& rendererUniforms, const std::vector<UserUniformInfo>& userUniforms)
		: m_Shader(vertexSource, fragmentSource)
	{
		m_RendererUniforms = GetUniformLocations(m_Shader, rendererUniforms);
		m_UserUniforms = GetUniformLocations(m_Shader, userUniforms);
	}

	ShaderInstance::ShaderInstance(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource, const std::vector<RendererUniformInfo>& rendererUniforms, const std::vector<UserUniformInfo>& userUniforms)
		: m_Shader(vertexSource, geometrySource, fragmentSource)
	{
		m_RendererUniforms = GetUniformLocations(m_Shader, rendererUniforms);
		m_UserUniforms = GetUniformLocations(m_Shader, userUniforms);
	}

	const Shader& ShaderInstance::GetShader() const
	{
		return m_Shader;
	}

	const std::vector<RendererUniformLocation>& ShaderInstance::GetRendererUniforms() const
	{
		return m_RendererUniforms;
	}

	const std::vector<UserUniformLocation>& ShaderInstance::GetUserUniforms() const
	{
		return m_UserUniforms;
	}

	std::vector<RendererUniformLocation> ShaderInstance::GetUniformLocations(Shader& shader, const std::vector<RendererUniformInfo>& uniforms)
	{
		std::vector<RendererUniformLocation> result;
		for (const RendererUniformInfo& uniform : uniforms)
		{
			RendererUniformLocation loc = { shader.GetUniformLocation(ShaderProgramFactory::GetRendererUniformName(uniform.Uniform)), uniform.Uniform };
			BLT_ASSERT(loc.Location != -1, "Unabled to find renderer uniform with name " + ShaderProgramFactory::GetRendererUniformName(uniform.Uniform));
			result.push_back(std::move(loc));
		}
		return result;
	}

	std::vector<UserUniformLocation> ShaderInstance::GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms)
	{
		std::vector<UserUniformLocation> result;
		for (const UserUniformInfo& uniform : uniforms)
		{
			UserUniformLocation loc = { uniform.LinkName, shader.GetUniformLocation(uniform.UniformName), uniform.Type };
			BLT_ASSERT(loc.Location != -1, "Unabled to find user uniform with name " + uniform.UniformName);
			result.push_back(std::move(loc));
		}
		return result;
	}

}