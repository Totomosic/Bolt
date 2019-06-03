#include "bltpch.h"
#include "MaterialGraphContext.h"

namespace Bolt
{

	MaterialGraphContext::MaterialGraphContext()
		: m_VertexPosition(ShaderStream::Position), m_VertexNormal(ShaderStream::Normal), m_VertexTexCoord(ShaderStream::TexCoord)
	{
		
	}

	const AttributeNode& MaterialGraphContext::VertexPosition() const
	{
		return m_VertexPosition;
	}

	const AttributeNode& MaterialGraphContext::VertexNormal() const
	{
		return m_VertexNormal;
	}

	const AttributeNode& MaterialGraphContext::VertexTexCoord() const
	{
		return m_VertexTexCoord;
	}

}