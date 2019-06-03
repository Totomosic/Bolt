#include "bltpch.h"
#include "PropertyNode.h"
#include "../../MaterialGraphBuilder.h"

namespace Bolt
{

	PropertyNode::PropertyNode(const blt::string& propertyName, ValueType type, const std::shared_ptr<UniformValueContainer>& defaultValue) : MaterialNode(ShaderStageCompatibility::All, 0, 1),
		m_PropertyName(propertyName), m_Type(type), m_DefaultValue(defaultValue)
	{
		OutputPort port(type);
		SetOutput(0, port);
	}

	const blt::string& PropertyNode::GetName() const
	{
		return m_PropertyName;
	}

	void PropertyNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		ShaderProgram& program = builder.GetProgram();
		ShaderVariablePtr var = program.Uniform(GetName(), m_Type, m_DefaultValue);
		node.BuildOutput(0, var);
	}

	void PropertyNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
	
	}

}