#include "bltpch.h"
#include "MultiplyNode.h"
#include "../../MaterialGraph.h"
#include "../Defaults/ValueNodes.h"

namespace Bolt
{

	MultiplyNode::MultiplyNode() : MaterialNode(ShaderStageCompatibility::All, 2, 1)
	{
		
	}

	void MultiplyNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::Mul(inputs.GetInput(0), inputs.GetInput(1)));
	}

	void MultiplyNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		FloatNode& node = (FloatNode&)graph.AddNode(std::make_unique<FloatNode>(1.0f));
		Connect(0, node.GetConnection(0));
		Connect(1, node.GetConnection(0));
	}

}