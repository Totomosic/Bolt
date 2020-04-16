#include "bltpch.h"
#include "MaterialGraphContext.h"
#include "Graphics/Buffers/BufferLayout.h"

namespace Bolt
{

	MaterialGraphContext::MaterialGraphContext()
		: m_VertexPosition(BufferLayout::DefaultIndices.Position), m_VertexNormal(BufferLayout::DefaultIndices.Normal), m_VertexTexCoord(BufferLayout::DefaultIndices.TexCoord)
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