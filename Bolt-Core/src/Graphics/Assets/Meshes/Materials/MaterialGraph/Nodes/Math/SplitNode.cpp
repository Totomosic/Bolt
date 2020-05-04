#include "bltpch.h"
#include "SplitNode.h"
#include "../../MaterialGraph.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	// SPLIT VECTOR2 NODE

	SplitVec2Node::SplitVec2Node() : MaterialNode(ShaderStageCompatibility::All, 1, 2)
	{
		MaterialNode::SetInput(0, InputPort(ValueType::Vector2f));
		SetOutput(0, OutputPort(ValueType::Float));
		SetOutput(1, OutputPort(ValueType::Float));
	}

	NodeConnection SplitVec2Node::GetR() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitVec2Node::GetG() const
	{
		return GetConnection(1);
	}

	void SplitVec2Node::SetInput(const NodeConnection& value)
	{
		Connect(0, value);
	}

	void SplitVec2Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::y(inputs.GetInput(0)));
	}

	void SplitVec2Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<ConstantVec2Node>(Vector2f(0.0f, 0.0f))).GetValue());
	}

	// SPLIT VECTOR3 NODE

	SplitVec3Node::SplitVec3Node() : MaterialNode(ShaderStageCompatibility::All, 1, 6)
	{
		MaterialNode::SetInput(0, InputPort(ValueType::Vector3f));
		SetOutput(0, ValueType::Float);
		SetOutput(1, ValueType::Float);
		SetOutput(2, ValueType::Float);
		SetOutput(3, ValueType::Vector2f);
		SetOutput(4, ValueType::Vector2f);
		SetOutput(5, ValueType::Vector2f);
	}

	NodeConnection SplitVec3Node::GetR() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitVec3Node::GetG() const
	{
		return GetConnection(1);
	}

	NodeConnection SplitVec3Node::GetB() const
	{
		return GetConnection(2);
	}

	NodeConnection SplitVec3Node::GetRG() const
	{
		return GetConnection(3);
	}

	NodeConnection SplitVec3Node::GetGB() const
	{
		return GetConnection(4);
	}

	NodeConnection SplitVec3Node::GetRB() const
	{
		return GetConnection(5);
	}

	void SplitVec3Node::SetInput(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SplitVec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::y(inputs.GetInput(0)));
		node.BuildOutput(2, ShaderFuncs::z(inputs.GetInput(0)));
		node.BuildOutput(3, ShaderFuncs::xy(inputs.GetInput(0)));
		node.BuildOutput(4, ShaderFuncs::yz(inputs.GetInput(0)));
		node.BuildOutput(5, ShaderFuncs::xz(inputs.GetInput(0)));
	}

	void SplitVec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<ConstantVec3Node>(Vector3f(0.0f, 0.0f, 0.0f))).GetValue());
	}

	// SPLIT VECTOR4 NODE

	SplitVec4Node::SplitVec4Node() : MaterialNode(ShaderStageCompatibility::All, 1, 8)
	{
		MaterialNode::SetInput(0, InputPort(ValueType::Vector4f));
		SetOutput(0, OutputPort(ValueType::Vector3f));
		SetOutput(1, OutputPort(ValueType::Vector2f));
		SetOutput(2, OutputPort(ValueType::Vector2f));
		SetOutput(3, OutputPort(ValueType::Vector2f));
		SetOutput(4, OutputPort(ValueType::Float));
		SetOutput(5, OutputPort(ValueType::Float));
		SetOutput(6, OutputPort(ValueType::Float));
		SetOutput(7, OutputPort(ValueType::Float));
	}

	NodeConnection SplitVec4Node::GetRGB() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitVec4Node::GetRG() const
	{
		return GetConnection(1);
	}

	NodeConnection SplitVec4Node::GetGB() const
	{
		return GetConnection(2);
	}

	NodeConnection SplitVec4Node::GetRB() const
	{
		return GetConnection(3);
	}

	NodeConnection SplitVec4Node::GetR() const
	{
		return GetConnection(4);
	}

	NodeConnection SplitVec4Node::GetG() const
	{
		return GetConnection(5);
	}

	NodeConnection SplitVec4Node::GetB() const
	{
		return GetConnection(6);
	}

	NodeConnection SplitVec4Node::GetA() const
	{
		return GetConnection(7);
	}

	void SplitVec4Node::SetInput(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SplitVec4Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::xyz(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::xy(inputs.GetInput(0)));
		node.BuildOutput(2, ShaderFuncs::yz(inputs.GetInput(0)));
		node.BuildOutput(3, ShaderFuncs::xz(inputs.GetInput(0)));
		node.BuildOutput(4, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(5, ShaderFuncs::y(inputs.GetInput(0)));
		node.BuildOutput(6, ShaderFuncs::z(inputs.GetInput(0)));
		node.BuildOutput(7, ShaderFuncs::w(inputs.GetInput(0)));
	}

	void SplitVec4Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<ConstantVec4Node>(Vector4f(0.0f, 0.0f, 0.0f, 0.0f))).GetValue());
	}

}