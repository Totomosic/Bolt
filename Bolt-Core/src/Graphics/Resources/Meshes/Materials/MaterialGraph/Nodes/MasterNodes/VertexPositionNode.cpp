#include "bltpch.h"
#include "VertexPositionNode.h"
#include "../../MaterialGraph.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	VertexPositionNode::VertexPositionNode() : MasterNode(ShaderStageCompatibility::VertexOnly, ValueType::Vector3f)
	{
	
	}

	void VertexPositionNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, context.VertexPosition().GetConnection(0));
	}

}