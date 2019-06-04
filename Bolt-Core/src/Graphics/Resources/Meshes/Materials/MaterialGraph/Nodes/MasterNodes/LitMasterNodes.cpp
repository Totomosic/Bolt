#include "bltpch.h"
#include "LitMasterNodes.h"
#include "../../MaterialGraph.h"

#include "../Defaults/ValueNodes.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	// SHININESS NODE

	ShininessNode::ShininessNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void ShininessNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(0.0f)).GetValue());
	}

	// SHINE DAMPER NODE

	ShineDamperNode::ShineDamperNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void ShineDamperNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(10.0f)).GetValue());
	}

}