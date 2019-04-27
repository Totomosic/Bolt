#include "Types.h"
#include "ShaderInstance.h"

namespace Bolt
{

	ShaderInstance::ShaderInstance(const CompiledShaderProgram& vertex, const CompiledShaderProgram& fragment)
		: m_Shader(vertex.Source, fragment.Source), VertexSource(vertex.Source), GeometrySource(), FragmentSource(fragment.Source)
	{
		m_RendererUniforms = GetUniformLocations(m_Shader, ConcatVectors(vertex.RendererUniforms, fragment.RendererUniforms));
		m_UserUniforms = GetUniformLocations(m_Shader, ConcatVectors(vertex.UserUniforms, fragment.UserUniforms));
	}

	ShaderInstance::ShaderInstance(const CompiledShaderProgram& vertex, const CompiledShaderProgram& geometry, const CompiledShaderProgram& fragment)
		: m_Shader(vertex.Source, geometry.Source, fragment.Source), VertexSource(vertex.Source), GeometrySource(geometry.Source), FragmentSource(fragment.Source)
	{
		m_RendererUniforms = GetUniformLocations(m_Shader, ConcatVectors(ConcatVectors(vertex.RendererUniforms, fragment.RendererUniforms), geometry.RendererUniforms));
		m_UserUniforms = GetUniformLocations(m_Shader, ConcatVectors(ConcatVectors(vertex.UserUniforms, fragment.UserUniforms), geometry.UserUniforms));
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
			RendererUniformLocation loc = { shader.GetUniformLocation(uniform.VarName), uniform.Uniform };
			BLT_ASSERT(loc.Location != -1, "Unable to find renderer uniform with name " + uniform.VarName);
			result.push_back(std::move(loc));
		}
		return result;
	}

	std::vector<UserUniformLocation> ShaderInstance::GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms)
	{
		std::vector<UserUniformLocation> result;
		for (const UserUniformInfo& uniform : uniforms)
		{
			UserUniformLocation loc = { uniform.LinkName, shader.GetUniformLocation(uniform.VarName), uniform.Type };
			BLT_ASSERT(loc.Location != -1, "Unable to find user uniform with name " + uniform.VarName);
			result.push_back(std::move(loc));
		}
		return result;
	}

}