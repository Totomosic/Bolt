#pragma once
#include "../MasterNode.h"

namespace Bolt
{

	class BLT_API VertexPositionNode : public MasterNode
	{
	public:
		VertexPositionNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API AlbedoNode : public MasterNode
	{
	public:
		AlbedoNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API NormalNode : public MasterNode
	{
	public:
		NormalNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API MetallicNode : public MasterNode
	{
	public:
		MetallicNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API RoughnessNode : public MasterNode
	{
	public:
		RoughnessNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API OcclusionNode : public MasterNode
	{
	public:
		OcclusionNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API AlphaNode : public MasterNode
	{
	public:
		AlphaNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API AlphaThresholdNode : public MasterNode
	{
	public:
		AlphaThresholdNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}