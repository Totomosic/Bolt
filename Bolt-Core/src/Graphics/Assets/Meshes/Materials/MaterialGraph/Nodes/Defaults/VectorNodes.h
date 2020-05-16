#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API ConstantVec2Node : public MaterialNode
	{
	private:
		Vector2f m_Value;

	public:
		ConstantVec2Node(const Vector2f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API ConstantVec3Node : public MaterialNode
	{
	private:
		Vector3f m_Value;

	public:
		ConstantVec3Node(const Vector3f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API ConstantVec4Node : public MaterialNode
	{
	private:
		Vector4f m_Value;

	public:
		ConstantVec4Node(const Vector4f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API Vec3Node : public MaterialNode
	{
	public:
		Vec3Node();

		NodeConnection GetValue() const;

		void SetXY(const NodeConnection& value);
		void SetZ(const NodeConnection& value);

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}