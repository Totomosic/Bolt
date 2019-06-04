#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API MultiplyNode : public MaterialNode
	{
	public:
		MultiplyNode();

		void SetInputA(const NodeConnection& connection);
		void SetInputB(const NodeConnection& connection);
		NodeConnection GetResult() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}