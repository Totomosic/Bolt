#include "bltpch.h"
#include "VectorNodes.h"
#include "../../../Shaders/Components/Values/ShaderLiteral.h"

namespace Bolt
{

	// VECTOR2 NODE

	Vec2Node::Vec2Node(const Vector2f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector2f);
		SetOutput(0, port);
	}

	NodeConnection Vec2Node::GetValue() const
	{
		return GetConnection(0);
	}

	void Vec2Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec2(m_Value));
	}

	void Vec2Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

	// VECTOR3 NODE

	Vec3Node::Vec3Node(const Vector3f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector3f);
		SetOutput(0, port);
	}

	NodeConnection Vec3Node::GetValue() const
	{
		return GetConnection(0);
	}

	void Vec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec3(m_Value));
	}

	void Vec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
	
	}

	// VECTOR4 NODE

	Vec4Node::Vec4Node(const Vector4f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector4f);
		SetOutput(0, port);
	}

	NodeConnection Vec4Node::GetValue() const
	{
		return GetConnection(0);
	}

	void Vec4Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec4(m_Value));
	}

	void Vec4Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

}