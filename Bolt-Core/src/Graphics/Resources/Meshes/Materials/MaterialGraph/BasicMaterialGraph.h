#pragma once
#include "MaterialGraph.h"

namespace Bolt
{

	class BLT_API BasicMaterialGraph : public MaterialGraph
	{
	private:
		MasterNode* m_VertexPosition;
		MasterNode* m_Color;
		MasterNode* m_Alpha;
		MasterNode* m_AlphaThreshold;

	public:
		BasicMaterialGraph();

		void SetVertexPosition(const NodeConnection& connection);
		void SetRGB(const NodeConnection& connection);
		void SetAlpha(const NodeConnection& connection);
		void SetAlphaThreshold(const NodeConnection& connection);

	protected:
		virtual void FinaliseBuild(const std::unordered_map<blt::string, ShaderValuePtr>& masterNodeValues) override;

	};

}