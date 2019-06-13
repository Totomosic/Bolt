#pragma once
#include "../MaterialNode.h"

namespace Bolt
{

	BLT_API enum class SampleMode
	{
		Color,
		Normal
	};

	class BLT_API SampleTextureNode : public MaterialNode
	{
	private:
		SampleMode m_Mode;

	public:
		SampleTextureNode(SampleMode mode = SampleMode::Color);

		SampleMode GetSampleMode() const;

		void SetSampleMode(SampleMode mode);
		void SetTexture(const NodeConnection& connection);
		void SetTexCoord(const NodeConnection& connection);
		NodeConnection GetRGBA() const;
		NodeConnection GetRGB() const;
		NodeConnection GetR() const;
		NodeConnection GetG() const;
		NodeConnection GetB() const;
		NodeConnection GetA() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;
	};

}