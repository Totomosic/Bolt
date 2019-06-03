#include "bltpch.h"
#include "MaterialGraph.h"
#include "../Material.h"

namespace Bolt
{

	MaterialGraph::MaterialGraph()
		: m_MasterNodes(), m_Nodes(), m_Builder()
	{
		
	}

	const std::vector<std::unique_ptr<MasterNode>>& MaterialGraph::GetMasterNodes() const
	{
		return m_MasterNodes;
	}

	const std::vector<std::unique_ptr<MaterialNode>>& MaterialGraph::GetNodes() const
	{
		return m_Nodes;
	}

	const MaterialGraphBuilder& MaterialGraph::GetBuilder() const
	{
		return m_Builder;
	}

	MaterialGraphBuilder& MaterialGraph::GetBuilder()
	{
		return m_Builder;
	}

	MaterialNode& MaterialGraph::AddNode(std::unique_ptr<MaterialNode>&& node)
	{
		MaterialNode* ptr = node.get();
		ptr->ConnectDefaults(*this, m_Builder.GetContext());
		m_Nodes.push_back(std::move(node));
		m_IsBuilt = false;
		return *ptr;
	}

	MasterNode& MaterialGraph::AddMasterNode(std::unique_ptr<MasterNode>&& masterNode)
	{
		MasterNode* ptr = masterNode.get();
		ptr->ConnectDefaults(*this, m_Builder.GetContext());
		m_MasterNodes.push_back(std::move(masterNode));
		m_IsBuilt = false;
		return *ptr;
	}

	void MaterialGraph::Build()
	{
		m_Builder.Reset();
		m_IsBuilt = true;
		std::vector<ShaderValuePtr> masterNodeResults;
		for (int i = 0; i < m_MasterNodes.size(); i++)
		{
			auto& masterNode = m_MasterNodes[i];
			masterNodeResults.push_back(m_Builder.BuildNode(*masterNode));
		}
		FinaliseBuild(masterNodeResults);
	}

	std::unique_ptr<Material> MaterialGraph::GetMaterial() const
	{
		BLT_ASSERT(m_IsBuilt, "MaterialGraph not built");
		return m_Builder.Build();
	}

}