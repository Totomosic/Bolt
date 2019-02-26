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
		ShaderFactory(bool useGeometry = false);

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

		template<typename T>
		ShaderValuePtr Literal(const T& value)
		{
			BLT_ASSERT(false, "Unable to create literal for type " + typeid(value).name());
			return ShaderValuePtr();
		}

		template<>
		ShaderValuePtr Literal(const int& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Int, GetCurrentShader(), std::to_string(value));
		}

		template<>
		ShaderValuePtr Literal(const float& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Float, GetCurrentShader(), std::to_string(value));
		}

		template<>
		ShaderValuePtr Literal(const Vector2f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector2f, GetCurrentShader(), "vec2(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ')');
		}

		template<>
		ShaderValuePtr Literal(const Vector2i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector2i, GetCurrentShader(), "vec2(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ')');
		}

		template<>
		ShaderValuePtr Literal(const Vector3f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector3f, GetCurrentShader(), "vec3(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ')');
		}

		template<>
		ShaderValuePtr Literal(const Vector3i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector3i, GetCurrentShader(), "vec3(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ')');
		}

		template<>
		ShaderValuePtr Literal(const Vector4f& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector4f, GetCurrentShader(), "vec4(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ',' + std::to_string(value.w) + ')');
		}

		template<>
		ShaderValuePtr Literal(const Vector4i& value)
		{
			return std::make_shared<ShaderValue>(OutputValueType::Constant, ValueType::Vector4i, GetCurrentShader(), "vec4(" + std::to_string(value.x) + ',' + std::to_string(value.y) + ',' + std::to_string(value.z) + ',' + std::to_string(value.w) + ')');
		}

		blt::string VertexSource() const;
		blt::string GeometrySource() const;
		blt::string FragmentSource() const;
		ShaderInstance CreateInstance() const;

	private:
		const ShaderProgramFactory& GetShaderProgram(ShaderType type) const;
		ShaderProgramFactory& GetShaderProgram(ShaderType type);
		const ShaderProgramFactory& CurrentShaderProgram() const;
		ShaderProgramFactory& CurrentShaderProgram();
		ShaderType GetShaderToPassTo(ShaderType currentShader) const;

	};

}