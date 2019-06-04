#pragma once
#include "../MasterNode.h"

namespace Bolt
{

	class BLT_API ShininessNode : public MasterNode
	{
	public:
		ShininessNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API ShineDamperNode : public MasterNode
	{
	public:
		ShineDamperNode();
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}