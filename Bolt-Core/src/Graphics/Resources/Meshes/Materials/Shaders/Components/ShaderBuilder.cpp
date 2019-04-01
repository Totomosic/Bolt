#include "Types.h"
#include "ShaderBuilder.h"
#include "Values/ShaderStream.h"

namespace Bolt
{

	ShaderBuilder::ShaderBuilder(ShaderType shaderType)
		: m_ShaderType(shaderType), m_Source("#version 430 core\n\n"), m_DeclaredVariables(), m_UniformCount(0), m_PassCount(0), m_VarCount(0), 
		m_StreamCursor(m_Source.size()), m_UniformCursor(m_Source.size()), m_PassCursor(m_Source.size()), m_CurrentCursor(0)
	{
		m_Source += "\nvoid main()\n{\n";
		m_CurrentCursor = m_Source.size();
		m_Source += "}";
	}

	ShaderType ShaderBuilder::GetShaderType() const
	{
		return m_ShaderType;
	}

	const blt::string& ShaderBuilder::GetSource() const
	{
		return m_Source;
	}

	blt::string ShaderBuilder::GetStreamName(int stream) const
	{
		switch (stream)
		{
		case ShaderStream::POSITION_INDEX:
			return "in_Position";
		case ShaderStream::NORMAL_INDEX:
			return "in_Normal";
		case ShaderStream::TEXCOORD_INDEX:
			return "in_TexCoord";
		case ShaderStream::COLOR_INDEX:
			return "in_Color";
		case ShaderStream::TANGENT_INDEX:
			return "in_Tangent";
		}
		BLT_ASSERT(false, "Cannot find stream index");
		return "";
	}

	int ShaderBuilder::SaveLineCursor() const
	{
		return m_Source.size() - m_CurrentCursor;
	}

	void ShaderBuilder::LoadLineCursor(int cursor)
	{
		m_CurrentCursor = m_Source.size() - cursor;
	}

	blt::string ShaderBuilder::GetUniformName() const
	{
		return "u_" + GetShaderTypeString() + '_' + std::to_string(m_UniformCount++);
	}

	blt::string ShaderBuilder::GetPassName() const
	{
		return "pass_" + GetShaderTypeString() + '_' + std::to_string(m_PassCount++);
	}

	blt::string ShaderBuilder::GetVariableName() const
	{
		return "var_" + std::to_string(m_VarCount++);
	}

	bool ShaderBuilder::IsDeclared(const ShaderVariable* variable) const
	{
		BLT_ASSERT(CanAccessVariable(variable), "Variable already belongs to another shader");
		return std::find(m_DeclaredVariables.begin(), m_DeclaredVariables.end(), variable) != m_DeclaredVariables.end();
	}

	void ShaderBuilder::WriteStreamLine(const blt::string& stream)
	{
		blt::string line = stream + ";\n";
		m_Source.insert(m_StreamCursor, line);
		m_StreamCursor += line.size();
		m_UniformCursor += line.size();
		m_PassCursor += line.size();
		m_CurrentCursor += line.size();
	}

	void ShaderBuilder::WriteUniformLine(const blt::string& uniform)
	{
		blt::string line = uniform + ";\n";
		m_Source.insert(m_UniformCursor, line);
		m_UniformCursor += line.size();
		m_PassCursor += line.size();
		m_CurrentCursor += line.size();
	}

	void ShaderBuilder::WritePassLine(const blt::string& pass)
	{
		blt::string line = pass + ";\n";
		m_Source.insert(m_PassCursor, line);
		m_PassCursor += line.size();
		m_CurrentCursor += line.size();
	}

	void ShaderBuilder::Write(const blt::string& str)
	{
		m_Source.insert(m_CurrentCursor, str);
		m_CurrentCursor += str.size();
	}

	void ShaderBuilder::PreviousLine()
	{
		m_CurrentCursor = m_Source.rfind('\n', m_CurrentCursor - 1) + 1;
	}

	void ShaderBuilder::NextLine()
	{
		Write('\n');
	}

	void ShaderBuilder::DeclareVariable(const ShaderVariable* variable)
	{
		BLT_ASSERT(CanAccessVariable(variable), "Variable already belongs to another shader");
		m_DeclaredVariables.push_back(variable);
		//variable->m_ShaderType = m_ShaderType;
	}

	blt::string ShaderBuilder::GetShaderTypeString() const
	{
		switch (m_ShaderType)
		{
		case ShaderType::Vertex:
			return "vertex";
		case ShaderType::Geometry:
			return "geometry";
		case ShaderType::Fragment:
			return "fragment";
		}
		BLT_ASSERT(false, "Unable to determine string for shader type");
		return "";
	}

	bool ShaderBuilder::CanAccessVariable(const ShaderVariable* variable) const
	{
		return variable->GetShaderType() == ShaderType::Ignore || variable->GetShaderType() == GetShaderType();
	}

}