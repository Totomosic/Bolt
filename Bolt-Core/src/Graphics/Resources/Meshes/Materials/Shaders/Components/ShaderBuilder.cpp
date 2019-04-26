#include "Types.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	ShaderBuilder::ShaderBuilder(ShaderType shaderType)
		: m_ShaderType(shaderType), m_Source("#version 430 core\n\n"), m_GlobalScope(), m_VarCount(0), m_CurrentScopeIndex(0), m_CurrentCursor(m_Source.size())
	{

	}

	ShaderType ShaderBuilder::GetShaderType() const
	{
		return m_ShaderType;
	}

	const blt::string& ShaderBuilder::GetSource() const
	{
		return m_Source;
	}

	const GlobalScope& ShaderBuilder::GetGlobalScope() const
	{
		return m_GlobalScope;
	}

	GlobalScope& ShaderBuilder::GetGlobalScope()
	{
		return m_GlobalScope;
	}

	int ShaderBuilder::SaveLineCursor() const
	{
		return m_Source.size() - m_CurrentCursor;
	}

	void ShaderBuilder::LoadLineCursor(int cursor)
	{
		m_CurrentCursor = m_Source.size() - cursor;
	}

	void ShaderBuilder::SetScopeIndex(int index)
	{
		m_CurrentScopeIndex = index;
	}

	blt::string ShaderBuilder::GetVariableName() const
	{
		return "var_" + std::to_string(m_VarCount++);
	}

	void ShaderBuilder::Write(const blt::string& str)
	{
		m_Source.insert(m_CurrentCursor, str);
		m_CurrentCursor += str.size();
	}

	void ShaderBuilder::Indent()
	{
		blt::string tabs = "";
		for (int i = 0; i < m_CurrentScopeIndex; i++)
		{
			tabs += '\n';
		}
		Write(tabs);
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

}