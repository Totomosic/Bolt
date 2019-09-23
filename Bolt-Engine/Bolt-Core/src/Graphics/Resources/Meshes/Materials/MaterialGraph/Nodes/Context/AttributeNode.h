#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API AttributeNode : public MaterialNode
	{
	private:
		int m_Stream;

	public:
		AttributeNode(int stream);

		int GetStream() const;
		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}