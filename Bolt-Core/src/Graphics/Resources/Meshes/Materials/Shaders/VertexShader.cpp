#include "bltpch.h"
#include "VertexShader.h"
#include "Components/ShaderBuilder.h"

namespace Bolt
{

	VertexShader::VertexShader(const BufferLayout& layout) : ShaderProgram(ShaderStage::Vertex),
		m_Layout(layout), m_Streams(), m_VertexPosition(nullptr)
	{
		m_VertexPosition = std::make_shared<ShaderVariable>(ValueType::Vector4f);
		m_VertexPosition->SetVarName("gl_Position");
		CreateStreams();
	}

	const BufferLayout& VertexShader::GetLayout() const
	{
		return m_Layout;
	}

	void VertexShader::SetBufferLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
		CreateStreams();
	}

	const ShaderVariablePtr& VertexShader::Stream(int streamIndex) const
	{
		return m_Streams.at(streamIndex);
	}

	void VertexShader::SetVertexPosition(const ShaderValuePtr& value)
	{
		SetVariable(m_VertexPosition, value);
	}

	CompiledShaderProgram VertexShader::Compile() const
	{
		CompiledShaderProgram result;
		result.Source = m_Builder.Build();
		CompileUniformVariables(result);
		m_Builder.Reset();
		return result;
	}

	void VertexShader::Reset()
	{
		ShaderProgram::Reset();
	}

	ShaderVariablePtr VertexShader::PrivateStream(int streamIndex)
	{
		return GetGlobalScope().DeclareVar(GetTypeOfShaderStream(streamIndex), "layout(location=" + std::to_string((int)streamIndex) + ") in");
	}

	ValueType VertexShader::GetTypeOfShaderStream(int streamIndex) const
	{
		BLT_ASSERT(m_Layout.HasAttribute(streamIndex), "Invalid attribute {}", streamIndex);
		const auto& stream = m_Layout.GetAttribute(streamIndex);
		DataType type = stream.Type;
		int count = stream.Count;
		if (type == DataType::Float)
		{
			switch (count)
			{
			case 1:
				return ValueType::Float;
			case 2:
				return ValueType::Vector2f;
			case 3: 
				return ValueType::Vector3f;
			case 4:
				return ValueType::Vector4f;
			}
		}
		else if (type == DataType::Int)
		{
			switch (count)
			{
			case 1:
				return ValueType::Int;
			}
		}
		else if (type == DataType::UByte)
		{
			if (stream.Normalised && count == 4)
			{
				return ValueType::Vector4f;
			}
		}
		BLT_ASSERT(false, "Invalid datatype");
		return ValueType::Void;
	}

	void VertexShader::CreateStreams()
	{
		m_Builder.Reset();
		m_Streams.clear();
		for (const auto& attribute : m_Layout.GetAttributes())
		{
			m_Streams[attribute.Index] = PrivateStream(attribute.Index);
		}
	}

}