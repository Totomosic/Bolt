#pragma once
#include "BuiltMaterialNode.h"
#include "MaterialNode.h"

#include "Nodes/Context/AttributeNode.h"

namespace Bolt
{

	class MaterialGraphBuilder;

	class BLT_API MaterialGraphContext
	{
	private:
		AttributeNode m_VertexPosition;
		AttributeNode m_VertexNormal;
		AttributeNode m_VertexTexCoord;
		
	public:
		MaterialGraphContext();

		const AttributeNode& VertexPosition() const;
		const AttributeNode& VertexNormal() const;
		const AttributeNode& VertexTexCoord() const;

	};

}