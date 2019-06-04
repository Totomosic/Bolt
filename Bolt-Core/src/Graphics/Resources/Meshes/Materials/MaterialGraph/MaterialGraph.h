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
		std::unordered_map<blt::string, std::unique_ptr<MasterNode>> m_MasterNodes;
		std::vector<std::unique_ptr<MaterialNode>> m_Nodes;
		MaterialGraphBuilder m_Builder;
		bool m_IsBuilt;

	public:
		MaterialGraph();
		virtual ~MaterialGraph() {}

		const std::unordered_map<blt::string, std::unique_ptr<MasterNode>>& GetMasterNodes() const;
		const std::vector<std::unique_ptr<MaterialNode>>& GetNodes() const;
		const MaterialGraphBuilder& GetBuilder() const;
		MaterialGraphBuilder& GetBuilder();

		MaterialNode& AddNode(std::unique_ptr<MaterialNode>&& node);

		template<typename T>
		PropertyNode& AddProperty(const blt::string& propertyName, const T& defaultValue = T())
		{
			return (PropertyNode&)AddNode(PropertyNode::Create<T>(propertyName, defaultValue));
		}

		void Build();
		std::unique_ptr<Material> GetMaterial() const;

	protected:
		MasterNode& AddMasterNode(const blt::string& name, std::unique_ptr<MasterNode>&& masterNode);
		virtual void FinaliseBuild(const std::unordered_map<blt::string, ShaderValuePtr>& masterNodeValues) = 0;

	};

}