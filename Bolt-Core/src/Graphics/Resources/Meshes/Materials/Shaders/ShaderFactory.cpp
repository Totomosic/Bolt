#include "Types.h"
#include "ShaderFactory.h"

namespace Bolt
{

	ShaderFactory::ShaderFactory(bool useGeometry)
		: m_Context(), m_Vertex(ShaderType::Vertex), m_Geometry(ShaderType::Geometry), m_Fragment(ShaderType::Fragment), m_UseGeometry(useGeometry), m_Shader(ShaderType::Vertex), m_RendererUniforms(), m_UserUniforms()
	{
		
	}

	ShaderType ShaderFactory::GetCurrentShader() const
	{
		return m_Shader;
	}

	void ShaderFactory::CurrentShader(ShaderType type)
	{
		BLT_ASSERT(!(!m_UseGeometry && type == ShaderType::Geometry), "Geometry shader was disabled");
		m_Shader = type;
	}

	bool ShaderFactory::IsUsingGeometryShader() const
	{
		return m_UseGeometry;
	}

	const BuiltInContext& ShaderFactory::Operations() const
	{
		return m_Context;
	}

	FunctionContext& ShaderFactory::VertexContext()
	{
		return m_Vertex.Context();
	}

	FunctionContext& ShaderFactory::GeometryContext()
	{
		return m_Geometry.Context();
	}

	FunctionContext& ShaderFactory::FragmentContext()
	{
		return m_Fragment.Context();
	}

	FunctionContext& ShaderFactory::CurrentContext()
	{
		return CurrentShaderProgram().Context();
	}

	ShaderValuePtr ShaderFactory::GetStream(ShaderStream stream)
	{
		BLT_ASSERT(GetCurrentShader() == ShaderType::Vertex, "Streams are only available in vertex shader");
		return m_Vertex.GetStream(stream);
	}

	ShaderValuePtr ShaderFactory::RendererUniform(Bolt::RendererUniform uniform)
	{
		ShaderValuePtr uniformPtr = CurrentShaderProgram().RendererUniform(uniform);
		m_RendererUniforms.push_back({ uniform });
		return std::move(uniformPtr);
	}

	ShaderValuePtr ShaderFactory::Uniform(const blt::string& linkName, ValueType type)
	{
		ShaderValuePtr uniform = CurrentShaderProgram().Uniform(type);
		m_UserUniforms.push_back({ linkName, uniform->VarName(), type });
		return std::move(uniform);
	}

	ShaderValuePtr ShaderFactory::FuncResult(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		return CurrentShaderProgram().FuncResult(declaration, inputs);
	}

	ShaderValuePtr ShaderFactory::FuncResult(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		return CurrentShaderProgram().FuncResult(declaration, inputs);
	}

	ShaderValuePtr ShaderFactory::Pass(ShaderValuePtr value)
	{
		ShaderValuePtr passValue = CurrentShaderProgram().Pass(value);
		ShaderProgramFactory& program = GetShaderProgram(GetShaderToPassTo(GetCurrentShader()));
		program.AddValuePassedToMe(passValue);
		return passValue;
	}

	void ShaderFactory::SetAttribute(ShaderAttribute attribute, ShaderValuePtr value)
	{
		return CurrentShaderProgram().SetAttribute(attribute, value);
	}

	blt::string ShaderFactory::VertexSource() const
	{
		return m_Vertex.CreateSource();
	}
	
	blt::string ShaderFactory::GeometrySource() const
	{
		BLT_ASSERT(IsUsingGeometryShader(), "Geometry shader is disabled");
		return m_Geometry.CreateSource();
	}

	blt::string ShaderFactory::FragmentSource() const
	{
		return m_Fragment.CreateSource();
	}

	ShaderInstance ShaderFactory::CreateInstance() const
	{
		if (IsUsingGeometryShader())
		{
			return ShaderInstance(VertexSource(), GeometrySource(), FragmentSource(), m_RendererUniforms, m_UserUniforms);
		}
		return ShaderInstance(VertexSource(), FragmentSource(), m_RendererUniforms, m_UserUniforms);
	}

	const ShaderProgramFactory& ShaderFactory::GetShaderProgram(ShaderType shader) const
	{
		switch (shader)
		{
		case ShaderType::Vertex:
			return m_Vertex;
		case ShaderType::Geometry:
			return m_Geometry;
		case ShaderType::Fragment:
			return m_Fragment;
		}
		BLT_ASSERT(false, "Unable to find shader program");
		return m_Vertex;
	}

	ShaderProgramFactory& ShaderFactory::GetShaderProgram(ShaderType shader)
	{
		switch (shader)
		{
		case ShaderType::Vertex:
			return m_Vertex;
		case ShaderType::Geometry:
			return m_Geometry;
		case ShaderType::Fragment:
			return m_Fragment;
		}
		BLT_ASSERT(false, "Unable to find shader program");
		return m_Vertex;
	}

	const ShaderProgramFactory& ShaderFactory::CurrentShaderProgram() const
	{
		return GetShaderProgram(GetCurrentShader());
	}

	ShaderProgramFactory& ShaderFactory::CurrentShaderProgram()
	{
		return GetShaderProgram(GetCurrentShader());
	}

	ShaderType ShaderFactory::GetShaderToPassTo(ShaderType currentShader) const
	{
		BLT_ASSERT(currentShader != ShaderType::Fragment, "Cannot pass from fragment shader");
		if (currentShader == ShaderType::Geometry)
		{
			return ShaderType::Fragment;
		}
		if (IsUsingGeometryShader())
		{
			return ShaderType::Geometry;
		}
		return ShaderType::Fragment;
	}

}