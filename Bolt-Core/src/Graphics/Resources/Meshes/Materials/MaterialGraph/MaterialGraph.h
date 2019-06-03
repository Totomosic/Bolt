#pragma once
#include "MaterialGraphBuilder.h"
#include "Nodes/MasterNode.h"
#include "Nodes/Context/PropertyNode.h"

#include "../Shaders/Components/Operations/MathOps/AddAssignOp.h"
#include "../Shaders/Components/Operations/MathOps/SubAssignOp.h"
#include "../Shaders/Components/Operations/MathOps/MulAssignOp.h"
#include "../Shaders/Components/Operations/MathOps/DivAssignOp.h"

namespace Bolt
{

	class BLT_API MaterialGraph
	{
	private:
		std::vector<std::unique_ptr<MasterNode>> m_MasterNodes;
		std::vector<std::unique_ptr<MaterialNode>> m_Nodes;
		MaterialGraphBuilder m_Builder;
		bool m_IsBuilt;

	public:
		MaterialGraph();
		virtual ~MaterialGraph() {}

		const std::vector<std::unique_ptr<MasterNode>>& GetMasterNodes() const;
		const std::vector<std::unique_ptr<MaterialNode>>& GetNodes() const;
		const MaterialGraphBuilder& GetBuilder() const;
		MaterialGraphBuilder& GetBuilder();

		MaterialNode& AddNode(std::unique_ptr<MaterialNode>&& node);
		MasterNode& AddMasterNode(std::unique_ptr<MasterNode>&& masterNode);

		void Build();
		std::unique_ptr<Material> GetMaterial() const;

		virtual void FinaliseBuild(const std::vector<ShaderValuePtr>& masterNodeValues) = 0;

	};

}