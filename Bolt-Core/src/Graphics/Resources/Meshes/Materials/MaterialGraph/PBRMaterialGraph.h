#pragma once
#include "MaterialGraph.h"

namespace Bolt
{

	class BLT_API PBRMaterialGraph : public MaterialGraph
	{
	private:
		MasterNode* m_VertexPosition;

	public:
		PBRMaterialGraph();

		void SetVertexPosition(const NodeConnection& connection);

		virtual void FinaliseBuild(const std::vector<ShaderValuePtr>& masterNodeValues) override;

	};

}