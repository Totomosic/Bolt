#pragma once
#include "Shader.h"
#include "Components/ShaderEnums.h"

namespace Bolt
{

	struct BLT_API UserUniformInfo
	{
	public:
		blt::string LinkName;
		blt::string UniformName;
		ValueType Type;
	};

	struct BLT_API UserUniformLocation
	{
	public:
		blt::string LinkName;
		int Location;
		ValueType Type;
	};

	struct BLT_API RendererUniformInfo
	{
	public:
		RendererUniform Uniform;
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
		ShaderInstance(const blt::string& vertexSource, const blt::string& fragmentSource, const std::vector<RendererUniformInfo>& rendererUniforms, const std::vector<UserUniformInfo>& userUniforms);
		ShaderInstance(const blt::string& vertexSource, const blt::string& geometrySource, const blt::string& fragmentSource, const std::vector<RendererUniformInfo>& rendererUniforms, const std::vector<UserUniformInfo>& userUniforms);

		const Shader& GetShader() const;
		const std::vector<RendererUniformLocation>& GetRendererUniforms() const;
		const std::vector<UserUniformLocation>& GetUserUniforms() const;

	private:
		static std::vector<RendererUniformLocation> GetUniformLocations(Shader& shader, const std::vector<RendererUniformInfo>& uniforms);
		static std::vector<UserUniformLocation> GetUniformLocations(Shader& shader, const std::vector<UserUniformInfo>& uniforms);

	};

}