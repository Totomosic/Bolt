#include "bltpch.h"
#include "MaterialGraphContext.h"
#include "Graphics/Buffers/BufferLayout.h"

namespace Bolt
{

	MaterialGraphContext::MaterialGraphContext()
		: m_VertexPosition(BufferLayout::POSITION_INDEX), m_VertexNormal(BufferLayout::NORMAL_INDEX), m_VertexTexCoord(BufferLayout::TEXCOORD_INDEX)
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