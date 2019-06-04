#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API SplitNode : public MaterialNode
	{
	public:
		SplitNode();

		NodeConnection GetRGB() const;
		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;
		NodeConnection GetA() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}