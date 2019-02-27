#pragma once
#include "ShaderProgramFactory.h"
#include "ShaderInstance.h"

namespace Bolt
{

	class BLT_API ShaderFactory
	{
	private:
		BuiltInContext m_Context;
		ShaderProgramFactory m_Vertex;
		ShaderProgramFactory m_Geometry;
		ShaderProgramFactory m_Fragment;
		bool m_UseGeometry;
		ShaderType m_Shader;

		std::vector<RendererUniformInfo> m_RendererUniforms;
		std::vector<UserUniformInfo> m_UserUniforms;

	public:
		ShaderFactory();

		ShaderType GetCurrentShader() const;
		void CurrentShader(ShaderType shader);
		bool IsUsingGeometryShader() const;

		const BuiltInContext& Operations() const;
		FunctionContext& VertexContext();
		FunctionContext& GeometryContext();
		FunctionContext& FragmentContext();
		FunctionContext& CurrentContext();

		ShaderValuePtr GetStream(ShaderStream stream);
		ShaderValuePtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderValuePtr Uniform(const blt::string& linkName, ValueType type);
		ShaderValuePtr FuncResult(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs);
		ShaderValuePtr FuncResult(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs);
		ShaderValuePtr Pass(ShaderValuePtr value);
		void SetAttribute(ShaderAttribute attribute, ShaderValuePtr value);

		blt::string VertexSource() const;
		blt::string GeometrySource() const;
		blt::string FragmentSource() const;
		ShaderInstance CreateInstance() const;
		void Reset();

	private:
		const ShaderProgramFactory& GetShaderProgram(ShaderType type) const;
		ShaderProgramFactory& GetShaderProgram(ShaderType type);
		const ShaderProgramFactory& CurrentShaderProgram() const;
		ShaderProgramFactory& CurrentShaderProgram();
		ShaderType GetShaderToPassTo(ShaderType currentShader) const;

		void EnableGeometry();

	};

}