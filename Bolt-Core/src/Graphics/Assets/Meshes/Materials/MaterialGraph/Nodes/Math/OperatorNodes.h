#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API AddNode : public MaterialNode
	{
	public:
		AddNode();

		void SetInputA(const NodeConnection& connection);
		void SetInputB(const NodeConnection& connection);
		NodeConnection GetResult() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API SubtractNode : public MaterialNode
	{
	public:
		SubtractNode();

		void SetInputA(const NodeConnection& connection);
		void SetInputB(const NodeConnection& connection);
		NodeConnection GetResult() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

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

	class BLT_API DivideNode : public MaterialNode
	{
	public:
		DivideNode();

		void SetInputA(const NodeConnection& connection);
		void SetInputB(const NodeConnection& connection);
		NodeConnection GetResult() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API PowerNode : public MaterialNode
	{
	public:
		PowerNode();

		void SetInputA(const NodeConnection& connection);
		void SetInputB(const NodeConnection& connection);
		NodeConnection GetResult() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}