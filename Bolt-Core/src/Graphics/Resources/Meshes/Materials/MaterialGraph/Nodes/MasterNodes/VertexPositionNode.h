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

}