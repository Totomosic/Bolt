#include "bltpch.h"
#include "MaterialGraph.h"
#include "../Material.h"

namespace Bolt
{

	MaterialGraph::MaterialGraph()
		: m_MasterNodes(), m_Nodes(), m_Builder()
	{
		
	}

	const std::unordered_map<blt::string, std::unique_ptr<MasterNode>>& MaterialGraph::GetMasterNodes() const
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

	void MaterialGraph::Build(bool isTransparent)
	{
		m_Builder.Reset();
		m_Builder.GetBuilder().SetIsTransparent(isTransparent);
		m_IsBuilt = true;
		std::unordered_map<blt::string, ShaderValuePtr> masterNodeResults;
		for (const auto& pair : m_MasterNodes)
		{
			masterNodeResults[pair.first] = m_Builder.BuildNode(*pair.second);
		}
		FinaliseBuild(masterNodeResults);
	}

	std::unique_ptr<Material> MaterialGraph::GetMaterial() const
	{
		BLT_ASSERT(m_IsBuilt, "MaterialGraph not built");
		return m_Builder.Build();
	}

	MasterNode& MaterialGraph::AddMasterNode(const blt::string& name, std::unique_ptr<MasterNode>&& masterNode)
	{
		BLT_ASSERT(m_MasterNodes.find(name) == m_MasterNodes.end(), "Master node with name {0} already exists", name);
		MasterNode* ptr = masterNode.get();
		ptr->ConnectDefaults(*this, m_Builder.GetContext());
		m_MasterNodes[name] = std::move(masterNode);
		m_IsBuilt = false;
		return *ptr;
	}

}