#pragma once
#include "../MaterialNode.h"

namespace Bolt
{

	class BLT_API MasterNode : public MaterialNode
	{
	public:
		MasterNode(ShaderStageCompatibility compatibility, ValueType type);

		void SetValue(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override = 0;
	};

}