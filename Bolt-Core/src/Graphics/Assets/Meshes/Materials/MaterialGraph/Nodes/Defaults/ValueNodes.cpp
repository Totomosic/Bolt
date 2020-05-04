#include "bltpch.h"
#include "ValueNodes.h"
#include "../../../Shaders/Components/Values/ShaderLiteral.h"

namespace Bolt
{

	ConstantFloatNode::ConstantFloatNode(float value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Float);
		SetOutput(0, port);
	}

	NodeConnection ConstantFloatNode::GetValue() const
	{
		return GetConnection(0);
	}

	void ConstantFloatNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromFloat(m_Value));
	}

	void ConstantFloatNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

}