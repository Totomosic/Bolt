#include "bltpch.h"
#include "VectorNodes.h"
#include "../../../Shaders/Components/Values/ShaderLiteral.h"

namespace Bolt
{

	Vec3Node::Vec3Node(const Vector3f& value) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_Value(value)
	{
		OutputPort port(ValueType::Vector3f);
		SetOutput(0, port);
	}

	const Vector3f& Vec3Node::GetValue() const
	{
		return m_Value;
	}

	void Vec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderLiteral::FromVec3(m_Value));
	}

	void Vec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
	
	}

}