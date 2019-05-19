#include "bltpch.h"
#include "LitMasterNodeSet.h"

namespace Bolt
{

	LitMasterNodeSet::LitMasterNodeSet()
		: m_Builder(), m_WorldPosition(), m_BaseColor(), m_WorldNormal(), m_Alpha(), m_AlphaThreshold()
	{
		VertexShader& vertex = m_Builder.Factory().Vertex();
		ShaderVariablePtr worldPosition = vertex.DeclareVar<Vector3f>();
		FragmentShader& fragment = m_Builder.Factory().Fragment();
	}

	void LitMasterNodeSet::SetPosition(const MaterialValue& value)
	{
		BLT_ASSERT(IsShaderCompatible(value.ShaderStage, ShaderType::Vertex), "VertexPosition must be able to be computed in the vertex shader");
		m_WorldPosition.CurrentValue = value;
	}

	void LitMasterNodeSet::SetBaseColor(const MaterialValue& value)
	{
		m_BaseColor.CurrentValue = value;
	}

	void LitMasterNodeSet::SetNormal(const MaterialValue& value)
	{
		m_WorldNormal.CurrentValue = value;
	}

	void LitMasterNodeSet::SetAlpha(const MaterialValue& value)
	{
		m_Alpha.CurrentValue = value;
	}

	void LitMasterNodeSet::SetAlphaThreshold(const MaterialValue& value)
	{
		m_AlphaThreshold.CurrentValue = value;
	}

}