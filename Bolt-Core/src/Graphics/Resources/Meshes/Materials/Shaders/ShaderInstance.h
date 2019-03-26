#pragma once
#include "Shader.h"
#include "ShaderProgram.h"

namespace Bolt
{

	struct BLT_API UserUniformLocation
	{
	public:
		blt::string LinkName;
		int Location;
		ValueType Type;
	};

	struct BLT_API RendererUniformLocation
	{
	public:
		int Location;
		RendererUniform Uniform;
	};

	class BLT_API ShaderInstance
	{
	private:
		Shader m_Shader;
		std::vector<RendererUniformLocation> m_RendererUniforms;
		std::vector<UserUniformLocation> m_UserUniforms;

	public:
		blt::string VertexSource;
		blt::string GeometrySource;
		blt::string FragmentSource;

	public:
		ShaderInstance(const CompiledShaderProgram& vertex, const CompiledShaderProgram& fragment);
		ShaderInstance(const CompiledShaderProgram& vertex, const CompiledShaderProgram& geometry, const CompiledShaderProgram& fragment);

		const Shader& GetShader() const;
		const std::vector<RendererUniformLocation>& GetRendererUniforms() const;
		const std::vector<UserUniformLocation>& GetUserUniforms() const;

	private:
		static std::vector<RendererUniformLocation> GetUniformLocations(Shader& shader, const std::vector<RendererUniformInfo>& uniforms);
		static std::vector<UserUniformLocation> GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms);

	};

}