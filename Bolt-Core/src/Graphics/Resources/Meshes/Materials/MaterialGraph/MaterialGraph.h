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

	class ResourceManager;

	class BLT_API MaterialGraph
	{
	private:
		ResourceManager* m_Resources;

		std::unordered_map<blt::string, std::unique_ptr<MasterNode>> m_MasterNodes;
		std::vector<std::unique_ptr<MaterialNode>> m_Nodes;
		MaterialGraphBuilder m_Builder;
		bool m_IsBuilt;

	public:
		MaterialGraph(ResourceManager* resourceManager);
		virtual ~MaterialGraph() {}

		ResourceManager& GetResourceManager() const;
		const std::unordered_map<blt::string, std::unique_ptr<MasterNode>>& GetMasterNodes() const;
		const std::vector<std::unique_ptr<MaterialNode>>& GetNodes() const;
		const MaterialGraphBuilder& GetBuilder() const;
		MaterialGraphBuilder& GetBuilder();
		const MaterialGraphContext& GetContext() const;

		template<typename T>
		T& AddNode()
		{
			return AddNode<T>(std::make_unique<T>());
		}

		template<typename T>
		T& AddNode(std::unique_ptr<T>&& node)
		{
			return (T&)AddNode((std::unique_ptr<MaterialNode>)std::move(node));
		}

		template<typename T>
		PropertyNode& AddProperty(const blt::string& propertyName, const T& defaultValue = T())
		{
			return (PropertyNode&)AddNode(PropertyNode::Create<T>(propertyName, defaultValue));
		}

		void Build(bool isTransparent = false);
		std::unique_ptr<Material> GetMaterial() const;

	protected:
		MaterialNode& AddNode(std::unique_ptr<MaterialNode>&& node);
		MasterNode& AddMasterNode(const blt::string& name, std::unique_ptr<MasterNode>&& masterNode);
		virtual void FinaliseBuild(const std::unordered_map<blt::string, ShaderValuePtr>& masterNodeValues) = 0;

	};

}