#pragma once
#include "../../MaterialNode.h"

namespace Bolt
{

	class BLT_API Vec3Node : public MaterialNode
	{
	private:
		Vector3f m_Value;

	public:
		Vec3Node(const Vector3f& value);

		const Vector3f& GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}