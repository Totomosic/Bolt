#include "bltpch.h"
#include "ShaderInstance.h"
#include "Core/Functions.h"

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
			if (GetTypeDimOfRendererUniform(uniform.Uniform) == ValueTypeDim::Array)
			{
				for (int i = 0; i < uniform.Length; i++)
				{
					blt::string arrPart = '[' + std::to_string(i) + ']';
					RendererUniformLocation loc = { shader.GetUniformLocation(uniform.VarName + arrPart), uniform.Uniform, false, i };
					if (loc.Location == -1)
					{
						BLT_CORE_WARN("Unable to find renderer uniform with name {}, it may not exist or may not be in use", uniform.VarName + arrPart);
					}
					result.push_back(std::move(loc));
				}
			}
			else
			{
				RendererUniformLocation loc = { shader.GetUniformLocation(uniform.VarName), uniform.Uniform, true, -1 };
				BLT_ASSERT(loc.Location != -1, "Unable to find renderer uniform with name " + uniform.VarName);
				result.push_back(std::move(loc));
			}
		}
		return result;
	}

	std::vector<UserUniformLocation> ShaderInstance::GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms)
	{
		std::vector<UserUniformLocation> result;
		for (const UserUniformInfo& uniform : uniforms)
		{
			if (uniform.Dimension == ValueTypeDim::Array)
			{
				for (int i = 0; i < uniform.Length; i++)
				{
					blt::string arrPart = '[' + std::to_string(i) + ']';
					UserUniformLocation loc = { uniform.LinkName + arrPart, shader.GetUniformLocation(uniform.VarName + arrPart), uniform.Type, false, uniform.DefaultValue };
					BLT_ASSERT(loc.Location != -1, "Unable to find user uniform with name " + uniform.VarName + arrPart);
					result.push_back(std::move(loc));
				}
			}
			else
			{
				UserUniformLocation loc = { uniform.LinkName, shader.GetUniformLocation(uniform.VarName), uniform.Type, true, uniform.DefaultValue };
				BLT_ASSERT(loc.Location != -1, "Unable to find user uniform with name " + uniform.VarName);
				result.push_back(std::move(loc));
			}
		}
		return result;
	}

}