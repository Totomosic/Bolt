#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API SplitVec3Node : public MaterialNode
	{
	public:
		SplitVec3Node();

		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;

		void SetInput(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API SplitVec4Node : public MaterialNode
	{
	public:
		SplitVec4Node();

		NodeConnection GetRGB() const;
		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;
		NodeConnection GetA() const;

		void SetInput(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}