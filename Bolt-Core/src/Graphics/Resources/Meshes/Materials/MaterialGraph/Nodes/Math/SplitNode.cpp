#include "bltpch.h"
#include "SplitNode.h"
#include "../../MaterialGraph.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	SplitNode::SplitNode() : MaterialNode(ShaderStageCompatibility::All, 1, 5)
	{

	}

	NodeConnection SplitNode::GetRGB() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitNode::GetR() const
	{
		return GetConnection(1);
	}

	NodeConnection SplitNode::GetG() const
	{
		return GetConnection(2);
	}

	NodeConnection SplitNode::GetB() const
	{
		return GetConnection(3);
	}

	NodeConnection SplitNode::GetA() const
	{
		return GetConnection(4);
	}

	void SplitNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::xyz(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(2, ShaderFuncs::y(inputs.GetInput(0)));
		node.BuildOutput(3, ShaderFuncs::z(inputs.GetInput(0)));
		node.BuildOutput(4, ShaderFuncs::w(inputs.GetInput(0)));
	}

	void SplitNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<Vec4Node>(Vector4f(0.0f, 0.0f, 0.0f, 0.0f))).GetConnection(0));
	}

}