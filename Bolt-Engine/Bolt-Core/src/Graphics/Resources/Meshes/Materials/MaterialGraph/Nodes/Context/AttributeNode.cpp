#include "bltpch.h"
#include "AttributeNode.h"
#include "../../MaterialGraphBuilder.h"

namespace Bolt
{

	AttributeNode::AttributeNode(int stream) : MaterialNode(ShaderStageCompatibility::VertexOnly, 0, 1),
		m_Stream(stream)
	{
	
	}

	int AttributeNode::GetStream() const
	{
		return m_Stream;
	}

	NodeConnection AttributeNode::GetValue() const
	{
		return GetConnection(0);
	}

	void AttributeNode::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		ShaderProgram& program = builder.GetProgram();
		BLT_ASSERT(program.Type() == ShaderStage::Vertex, "Can only operate in vertex shader");
		VertexShader& vertex = (VertexShader&)program;
		ShaderVariablePtr var = vertex.Stream(m_Stream);
		node.BuildOutput(0, var);
	}

	void AttributeNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		
	}

}