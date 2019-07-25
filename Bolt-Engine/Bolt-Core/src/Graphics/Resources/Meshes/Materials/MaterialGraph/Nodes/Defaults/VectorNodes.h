#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API Vec2Node : public MaterialNode
	{
	private:
		Vector2f m_Value;

	public:
		Vec2Node(const Vector2f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API Vec3Node : public MaterialNode
	{
	private:
		Vector3f m_Value;

	public:
		Vec3Node(const Vector3f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

	class BLT_API Vec4Node : public MaterialNode
	{
	private:
		Vector4f m_Value;

	public:
		Vec4Node(const Vector4f& value);

		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}