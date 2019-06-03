#include "bltpch.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	ShaderBuilder::ShaderBuilder(ShaderStage shaderType)
		: m_ShaderStage(shaderType), m_Source("#version 430 core\n"), m_GlobalScope(), m_MainScope(1), m_VarCount(0), m_PassCount(0), m_CurrentScopeIndex(0), m_CurrentCursor(m_Source.size())
	{
		Reset();
	}

	ShaderStage ShaderBuilder::GetShaderStage() const
	{
		return m_ShaderStage;
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

	const MainScope& ShaderBuilder::GetMainScope() const
	{
		return m_MainScope;
	}

	MainScope& ShaderBuilder::GetMainScope()
	{
		return m_MainScope;
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

	void ShaderBuilder::NextLine()
	{
#ifdef BLT_DEBUG
		Write("\n");
		Indent();
#endif
	}

	blt::string ShaderBuilder::GetVariableName() const
	{
		return "v" + GetShaderStageString() + std::to_string(m_VarCount++);
	}

	blt::string ShaderBuilder::GetPassName() const
	{
		return "p" + GetShaderStageString() + std::to_string(m_PassCount++);
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
			tabs += '\t';
		}
		Write(tabs);
	}

	blt::string ShaderBuilder::Build()
	{
		GetGlobalScope().Build(*this);
		GetMainScope().Build(*this);
		blt::string source = GetSource();
		Reset();
		return source;
	}

	blt::string ShaderBuilder::GetShaderStageString() const
	{
		switch (m_ShaderStage)
		{
		case ShaderStage::Vertex:
			return "v";
		case ShaderStage::Geometry:
			return "g";
		case ShaderStage::Fragment:
			return "f";
		}
		BLT_ASSERT(false, "Unable to determine string for shader type");
		return "";
	}

	void ShaderBuilder::Reset()
	{
		m_Source = "#version 430 core\n";
		m_CurrentCursor = m_Source.size();
		m_VarCount = 0;
		m_PassCount = 0;
	}

}