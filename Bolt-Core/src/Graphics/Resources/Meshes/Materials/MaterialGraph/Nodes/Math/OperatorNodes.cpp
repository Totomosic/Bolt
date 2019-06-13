#include "bltpch.h"
#include "OperatorNodes.h"
#include "../../MaterialGraph.h"
#include "../Defaults/ValueNodes.h"

namespace Bolt
{

	// ADD NODE

	AddNode::AddNode() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{

	}

	void AddNode::SetInputA(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void AddNode::SetInputB(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection AddNode::GetResult() const
	{
		return GetConnection(0);
	}

	void AddNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Add(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void AddNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		FloatNode& node = graph.AddNode(std::make_unique<FloatNode>(1.0f));
		Connect(0, node.GetValue());
		Connect(1, node.GetValue());
	}

	// SUBTRACT NODE

	SubtractNode::SubtractNode() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{

	}

	void SubtractNode::SetInputA(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SubtractNode::SetInputB(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection SubtractNode::GetResult() const
	{
		return GetConnection(0);
	}

	void SubtractNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Sub(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void SubtractNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		FloatNode& a = graph.AddNode(std::make_unique<FloatNode>(1.0f));
		FloatNode& b = graph.AddNode(std::make_unique<FloatNode>(0.0f));
		Connect(0, a.GetValue());
		Connect(1, b.GetValue());
	}

	// MULTIPLY NODE

	MultiplyNode::MultiplyNode() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{
		
	}

	void MultiplyNode::SetInputA(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void MultiplyNode::SetInputB(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection MultiplyNode::GetResult() const
	{
		return GetConnection(0);
	}

	void MultiplyNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Mul(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void MultiplyNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		FloatNode& node = graph.AddNode(std::make_unique<FloatNode>(1.0f));
		Connect(0, node.GetValue());
		Connect(1, node.GetValue());
	}

	// DIVIDE NODE

	DivideNode::DivideNode() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{

	}

	void DivideNode::SetInputA(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void DivideNode::SetInputB(const NodeConnection& connection)
	{
		Connect(1, connection);
	}

	NodeConnection DivideNode::GetResult() const
	{
		return GetConnection(0);
	}

	void DivideNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Div(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void DivideNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		FloatNode& node = graph.AddNode(std::make_unique<FloatNode>(1.0f));
		Connect(0, node.GetValue());
		Connect(1, node.GetValue());
	}

}