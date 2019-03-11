#include "Types.h"
#include "VertexShader.h"

namespace Bolt
{

	VertexShader::VertexShader() : ShaderProgram(ShaderType::Vertex),
		m_VertexPosition(nullptr)
	{
	
	}

	const ShaderStreamPtr& VertexShader::Position() const
	{
		return m_PositionStream;
	}

	const ShaderStreamPtr& VertexShader::Normal() const
	{
		return m_NormalStream;
	}

	const ShaderStreamPtr& VertexShader::TexCoord() const
	{
		return m_TexCoordStream;
	}

	const ShaderStreamPtr& VertexShader::Color() const
	{
		return m_ColorStream;
	}

	const ShaderStreamPtr& VertexShader::Tangent() const
	{
		return m_TangentStream;
	}

	void VertexShader::SetVertexPosition(ShaderValuePtr value)
	{
		m_VertexPosition = std::make_shared<ShaderAttribute>(ShaderAttribute::VERTEX_POSITION, value);
	}

	CompiledShaderProgram VertexShader::Compile() const
	{
		BLT_ASSERT(m_VertexPosition != nullptr, "VertexPosition attribute has not been set");
		CompiledShaderProgram result;
		std::unordered_map<const ShaderValue*, blt::string> namedValueMap;
		ShaderValueNameFunc getValueNameFunc = [&namedValueMap](const ShaderValue* value)
		{
			auto it = namedValueMap.find(value);
			if (it != namedValueMap.end())
			{
				return it->second;
			}
			blt::string name = "Value_" + std::to_string(namedValueMap.size());
			namedValueMap[value] = name;
			return name;
		};
		blt::string source = "#version 430 core\n\n";
		source += "layout(location = 0) vec4 " + getValueNameFunc(Position().get()) + ";\n";
		source += "layout(location = 1) vec3 " + getValueNameFunc(Normal().get()) + ";\n";
		source += "layout(location = 2) vec2 " + getValueNameFunc(TexCoord().get()) + ";\n";
		source += "layout(location = 3) vec4 " + getValueNameFunc(Color().get()) + ";\n";
		source += "layout(location = 4) vec3 " + getValueNameFunc(Tangent().get()) + ";\n\n";

		int currentUniform = 0;
		for (const ShaderRendererUniformPtr& ptr : m_RendererUniforms)
		{
			blt::string uniformName = "RU_" + std::to_string(currentUniform);
			result.NamedRendererUniforms[ptr->UniformType()] = uniformName;
			namedValueMap[ptr.get()] = uniformName;
			source += ValueTypeToGLSLString(ptr->Type()) + " " + uniformName + ";\n";
			currentUniform += 1;
		}
		currentUniform = 0;
		for (const ShaderUniformPtr& ptr : m_UserUniforms)
		{
			blt::string uniformName = "UU_" + std::to_string(currentUniform);
			result.NamedUserUniforms[ptr] = uniformName;
			namedValueMap[ptr.get()] = uniformName;
			source += ValueTypeToGLSLString(ptr->Type()) + " " + uniformName + ";\n";
			currentUniform += 1;
		}

		int currentPassValue = 0;

		result.Source = source;
		return result;
	}

	void VertexShader::Reset()
	{
		ShaderProgram::Reset();
		m_VertexPosition = nullptr;
	}

}