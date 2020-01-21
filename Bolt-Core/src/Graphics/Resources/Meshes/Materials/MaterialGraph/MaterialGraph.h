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

		std::unordered_map<std::string, std::unique_ptr<MasterNode>> m_MasterNodes;
		std::vector<std::unique_ptr<MaterialNode>> m_Nodes;
		MaterialGraphBuilder m_Builder;
		bool m_IsBuilt;

	public:
		MaterialGraph(ResourceManager* resourceManager);
		virtual ~MaterialGraph() {}

		ResourceManager& GetResourceManager() const;
		const std::unordered_map<std::string, std::unique_ptr<MasterNode>>& GetMasterNodes() const;
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
		PropertyNode& AddProperty(const std::string& propertyName, const T& defaultValue = T())
		{
			return (PropertyNode&)AddNode(PropertyNode::Create<T>(propertyName, defaultValue));
		}

		void Build(bool isTransparent = false);

		template<typename MaterialT = Material>
		std::unique_ptr<MaterialT> GetMaterial() const
		{
			BLT_ASSERT(m_IsBuilt, "MaterialGraph not built");
			return m_Builder.Build<MaterialT>();
		}

	protected:
		MaterialNode& AddNode(std::unique_ptr<MaterialNode>&& node);
		MasterNode& AddMasterNode(const std::string& name, std::unique_ptr<MasterNode>&& masterNode);
		virtual void FinaliseBuild(const std::unordered_map<std::string, ShaderValuePtr>& masterNodeValues) = 0;

	};

}