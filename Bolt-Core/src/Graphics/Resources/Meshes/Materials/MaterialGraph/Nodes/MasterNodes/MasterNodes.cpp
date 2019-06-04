#include "bltpch.h"
#include "MasterNodes.h"
#include "../../MaterialGraph.h"

#include "../Defaults/ValueNodes.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	// VERTEX POSITION NODE

	VertexPositionNode::VertexPositionNode() : MasterNode(ShaderStageCompatibility::VertexOnly, ValueType::Vector3f)
	{

	}

	void VertexPositionNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, context.VertexPosition().GetConnection(0));
	}

	// ALBEDO NODE

	AlbedoNode::AlbedoNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Vector3f)
	{

	}

	void AlbedoNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<Vec3Node>(Vector3f{ 1.0f, 1.0f, 1.0f })).GetConnection(0));
	}

	// NORMAL NODE

	NormalNode::NormalNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Vector3f)
	{

	}

	void NormalNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<Vec3Node>(Vector3f{ 0.0f, 1.0f, 0.0f })).GetConnection(0));
	}

	// METALLIC NODE

	MetallicNode::MetallicNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void MetallicNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(0.0f)).GetConnection(0));
	}

	// ROUGHNESS NODE

	RoughnessNode::RoughnessNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void RoughnessNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(0.5f)).GetConnection(0));
	}

	// OCCLUSION NODE

	OcclusionNode::OcclusionNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void OcclusionNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(1.0f)).GetConnection(0));
	}

	// ALPHA NODE

	AlphaNode::AlphaNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void AlphaNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(1.0f)).GetConnection(0));
	}

	// ALPHA THRESHOLD NODE

	AlphaThresholdNode::AlphaThresholdNode() : MasterNode(ShaderStageCompatibility::FragmentOnly, ValueType::Float)
	{

	}

	void AlphaThresholdNode::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<FloatNode>(0.0f)).GetConnection(0));
	}

}