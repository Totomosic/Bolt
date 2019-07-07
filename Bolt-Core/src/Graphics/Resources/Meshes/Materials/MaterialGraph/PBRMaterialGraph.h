#pragma once
#include "MaterialGraph.h"

namespace Bolt
{

	class BLT_API PBRMaterialGraph : public MaterialGraph
	{
	private:
		MasterNode* m_VertexPosition;
		MasterNode* m_Albedo;
		MasterNode* m_Normal;
		MasterNode* m_Metallic;
		MasterNode* m_Roughness;
		MasterNode* m_Occlusion;
		MasterNode* m_Alpha;
		MasterNode* m_AlphaThreshold;

	public:
		PBRMaterialGraph();

		void SetVertexPosition(const NodeConnection& connection);
		void SetRGB(const NodeConnection& connection);
		void SetNormal(const NodeConnection& connection);
		void SetMetallic(const NodeConnection& connection);
		void SetRoughness(const NodeConnection& connection);
		void SetOcclusion(const NodeConnection& connection);
		void SetAlpha(const NodeConnection& connection);
		void SetAlphaThreshold(const NodeConnection& connection);

	protected:
		virtual void FinaliseBuild(const std::unordered_map<blt::string, ShaderValuePtr>& masterNodeValues) override;

	};

}