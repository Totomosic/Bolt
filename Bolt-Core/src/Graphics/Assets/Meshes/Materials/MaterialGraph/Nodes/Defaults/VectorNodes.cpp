#include "bltpch.h"
#include "VectorNodes.h"
#include "../../../Shaders/Components/Values/ShaderLiteral.h"
#include "../../MaterialGraph.h"

namespace Bolt
{

	// ============================================================================================================================================================================
	// CONSTANT VECTOR2 NODE
	// ============================================================================================================================================================================

	ConstantVec2Node::ConstantVec2Node(const Vector2f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector2f);
		SetOutput(0, port);
	}

	NodeConnection ConstantVec2Node::GetValue() const
	{
		return GetConnection(0);
	}

	void ConstantVec2Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec2(m_Value));
	}

	void ConstantVec2Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

	// ============================================================================================================================================================================
	// CONSTANT VECTOR3 NODE
	// ============================================================================================================================================================================

	ConstantVec3Node::ConstantVec3Node(const Vector3f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector3f);
		SetOutput(0, port);
	}

	NodeConnection ConstantVec3Node::GetValue() const
	{
		return GetConnection(0);
	}

	void ConstantVec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec3(m_Value));
	}

	void ConstantVec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
	
	}

	// ============================================================================================================================================================================
	// CONSTANT VECTOR4 NODE
	// ============================================================================================================================================================================

	ConstantVec4Node::ConstantVec4Node(const Vector4f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector4f);
		SetOutput(0, port);
	}

	NodeConnection ConstantVec4Node::GetValue() const
	{
		return GetConnection(0);
	}

	void ConstantVec4Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec4(m_Value));
	}

	void ConstantVec4Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

	// ============================================================================================================================================================================
	// VECTOR3 NODE
	// ============================================================================================================================================================================

	Vec3Node::Vec3Node() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{
		SetOutput(0, OutputPort(ValueType::Vector3f));
	}

	NodeConnection Vec3Node::GetValue() const
	{
		return GetConnection(0);
	}

	void Vec3Node::SetXY(const NodeConnection& value)
	{
		Connect(0, value);
	}

	void Vec3Node::SetZ(const NodeConnection& value)
	{
		Connect(1, value);
	}

	void Vec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Vec3(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void Vec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<ConstantVec3Node>(Vector3f(0.0f, 0.0f, 0.0f))).GetValue());
	}

}