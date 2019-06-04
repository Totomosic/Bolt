#include "bltpch.h"
#include "MasterNode.h"
#include "../MaterialGraphBuilder.h"

namespace Bolt
{

	MasterNode::MasterNode(ShaderStageCompatibility compatibility, ValueType type) : MaterialNode(compatibility, 1, 1)
	{
		InputPort iPort(type);
		SetInput(0, iPort);
		OutputPort oPort(type);
		SetOutput(0, oPort);
	}

	void MasterNode::SetValue(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void MasterNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, builder.GetProgram().DefineVar(inputs.GetInput(0)));
	}

}