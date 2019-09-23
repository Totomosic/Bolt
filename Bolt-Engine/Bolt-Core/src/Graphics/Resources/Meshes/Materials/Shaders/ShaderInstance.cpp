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
		BLT_CORE_TRACE("VERTEX SOURCE\n{}\n", vertex.Source);
		BLT_CORE_TRACE("FRAGMENT SOURCE\n{}\n", fragment.Source);
	}

	ShaderInstance::ShaderInstance(const CompiledShaderProgram& vertex, const CompiledShaderProgram& geometry, const CompiledShaderProgram& fragment)
		: m_Shader(vertex.Source, geometry.Source, fragment.Source), VertexSource(vertex.Source), GeometrySource(geometry.Source), FragmentSource(fragment.Source)
	{
		m_RendererUniforms = GetUniformLocations(m_Shader, ConcatVectors(ConcatVectors(vertex.RendererUniforms, fragment.RendererUniforms), geometry.RendererUniforms));
		m_UserUniforms = GetUniformLocations(m_Shader, ConcatVectors(ConcatVectors(vertex.UserUniforms, fragment.UserUniforms), geometry.UserUniforms));
		BLT_CORE_TRACE("VERTEX SOURCE\n{}\n", vertex.Source);
		BLT_CORE_TRACE("GEOMETRY SOURCE\n{}\n", geometry.Source);
		BLT_CORE_TRACE("FRAGMENT SOURCE\n{}\n", fragment.Source);
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
					else
					{
						BLT_CORE_INFO("Found renderer uniform in shader {0}, {1} location = {2}", shader.Id(), uniform.VarName + arrPart, loc.Location);
					}
					result.push_back(std::move(loc));
				}
			}
			else
			{
				RendererUniformLocation loc = { shader.GetUniformLocation(uniform.VarName), uniform.Uniform, true, -1 };
				if (loc.Location == -1)
				{
					BLT_CORE_WARN("Unable to find renderer uniform with name " + uniform.VarName);
				}
				else
				{
					BLT_CORE_INFO("Found renderer uniform in shader {0}, {1} location = {2}", shader.Id(), uniform.VarName, loc.Location);
				}
				result.push_back(std::move(loc));
			}
		}
		return result;
	}

	std::vector<UserUniformLocation> ShaderInstance::GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms)
	{
		std::vector<UserUniformLocation> result;
		int textureCount = 0;
		for (const UserUniformInfo& uniform : uniforms)
		{
			if (uniform.Dimension == ValueTypeDim::Array)
			{
				for (int i = 0; i < uniform.Length; i++)
				{
					blt::string arrPart = '[' + std::to_string(i) + ']';
					UserUniformLocation loc = { uniform.LinkName + arrPart, shader.GetUniformLocation(uniform.VarName + arrPart), uniform.Type, false, textureCount, uniform.DefaultValue };
					if (loc.Location == -1)
					{
						BLT_CORE_WARN("Unable to find user uniform with name {0}{1}", uniform.VarName, arrPart);
					}
					else
					{
						BLT_CORE_INFO("Found user uniform in shader {0}, {1} location = {2}", shader.Id(), loc.LinkName, loc.Location);
					}
					result.push_back(std::move(loc));
					if (uniform.Type == ValueType::Texture2D)
					{
						textureCount++;
					}
				}
			}
			else
			{
				UserUniformLocation loc = { uniform.LinkName, shader.GetUniformLocation(uniform.VarName), uniform.Type, true, textureCount, uniform.DefaultValue };
				if (loc.Location == -1)
				{
					BLT_CORE_WARN("Unable to find user uniform with name {}", uniform.VarName);
				}
				else
				{
					BLT_CORE_INFO("Found user uniform in shader {0}, {1} location = {2}", shader.Id(), loc.LinkName, loc.Location);
				}
				result.push_back(std::move(loc));
				if (uniform.Type == ValueType::Texture2D)
				{
					textureCount++;
				}
			}
		}
		return result;
	}

}