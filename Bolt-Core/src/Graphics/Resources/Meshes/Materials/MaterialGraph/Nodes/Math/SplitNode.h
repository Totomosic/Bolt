#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API SplitVec2Node : public MaterialNode
	{
	public:
		SplitVec2Node();

		NodeConnection GetR() const;
		NodeConnection GetG() const;

		inline NodeConnection GetX() const { return GetR(); }
		inline NodeConnection GetY() const { return GetG(); }

		void SetInput(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API SplitVec3Node : public MaterialNode
	{
	public:
		SplitVec3Node();

		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;
		NodeConnection GetRG() const;
		NodeConnection GetGB() const;
		NodeConnection GetRB() const;

		inline NodeConnection GetXY() const { return GetRG(); }
		inline NodeConnection GetXZ() const { return GetRB(); }
		inline NodeConnection GetYZ() const { return GetGB(); }
		inline NodeConnection GetX() const { return GetR(); }
		inline NodeConnection GetY() const { return GetG(); }
		inline NodeConnection GetZ() const { return GetB(); }

		void SetInput(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API SplitVec4Node : public MaterialNode
	{
	public:
		SplitVec4Node();

		NodeConnection GetRGB() const;
		NodeConnection GetRG() const;
		NodeConnection GetGB() const;
		NodeConnection GetRB() const;
		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;
		NodeConnection GetA() const;

		inline NodeConnection GetXYZ() const { return GetRGB(); }
		inline NodeConnection GetXY() const { return GetRG(); }
		inline NodeConnection GetXZ() const { return GetRB(); }
		inline NodeConnection GetYZ() const { return GetGB(); }
		inline NodeConnection GetX() const { return GetR(); }
		inline NodeConnection GetY() const { return GetG(); }
		inline NodeConnection GetZ() const { return GetB(); }
		inline NodeConnection GetW() const { return GetA(); }

		void SetInput(const NodeConnection& connection);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}