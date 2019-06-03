#include "bltpch.h"
#include "ValueNodes.h"
#include "../../../Shaders/Components/Values/ShaderLiteral.h"

namespace Bolt
{

	FloatNode::FloatNode(float value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Float);
		SetOutput(0, port);
	}

	float FloatNode::GetValue() const
	{
		return m_Value;
	}

	void FloatNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromFloat(m_Value));
	}

	void FloatNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{

	}

}