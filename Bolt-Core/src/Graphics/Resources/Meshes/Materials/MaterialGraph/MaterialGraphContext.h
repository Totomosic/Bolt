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

		// Gets the object-space position (Vec3) of vertex provided by the model (without transformations)
		const AttributeNode& VertexPosition() const;
		// Gets the object-space normal (Vec3) of vertex provided by the model (without transformations)
		const AttributeNode& VertexNormal() const;
		// Gets the object-space texture coordinate (Vec2) provided by the model (without transformations)
		const AttributeNode& VertexTexCoord() const;

	};

}