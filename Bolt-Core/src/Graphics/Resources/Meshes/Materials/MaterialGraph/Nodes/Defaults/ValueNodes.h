#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API FloatNode : public MaterialNode
	{
	private:
		float m_Value;

	public:
		FloatNode(float value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}