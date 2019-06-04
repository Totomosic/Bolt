#include "bltpch.h"
#include "MaterialGraphBuilder.h"
#include "../Material.h"

namespace Bolt
{

	MaterialGraphBuilder::MaterialGraphBuilder()
		: m_Builder(), m_Context(), m_BuiltNodes(), m_CurrentProgram(nullptr)
	{
		Reset();
	}

	const MaterialGraphContext& MaterialGraphBuilder::GetContext() const
	{
		return m_Context;
	}

	ShaderProgram& MaterialGraphBuilder::GetProgram() const
	{
		BLT_ASSERT(m_CurrentProgram != nullptr, "Invalid current program");
		return *m_CurrentProgram;
	}

	MaterialBuilder& MaterialGraphBuilder::GetBuilder()
	{
		return m_Builder;
	}

	ShaderValuePtr MaterialGraphBuilder::BuildNode(const MasterNode& masterNode)
	{
		switch (masterNode.GetCompatibility())
		{
		case ShaderStageCompatibility::VertexOnly:
			m_CurrentProgram = &GetBuilder().Factory().Vertex();
			break;
		case ShaderStageCompatibility::FragmentOnly:
			m_CurrentProgram = &GetBuilder().Factory().Fragment();
			break;
		default:
			BLT_ASSERT(false, "Invalid shader stage");
		}
		std::vector<NodeInfo> nodeList = FlattenNode(masterNode);
		for (const NodeInfo& node : nodeList)
		{
			if (!IsAlreadyBuilt(node.NodePtr))
			{
				ShaderProgram* oldCurrentProgram = m_CurrentProgram;
				bool wasCompatible = true;
				if (!IsShaderCompatible(masterNode.GetCompatibility(), node.NodePtr->GetCompatibility()))
				{
					BLT_ASSERT(m_CurrentProgram->Type() == ShaderStage::Fragment, "Cannot be done, Incompatible node");
					wasCompatible = false;
					m_CurrentProgram = &GetBuilder().Factory().Vertex();
				}
				LinkedInputs inputs(node.NodePtr->GetInputCount());
				for (int i = 0; i < node.Dependencies.size(); i++)
				{
					const NodeDependency& dependency = node.Dependencies.at(i);
					BLT_ASSERT(IsAlreadyBuilt(dependency.NodePtr), "Invalid flatten order");
					const BuiltMaterialNode& builtNode = GetBuiltNode(dependency.NodePtr);
					inputs.SetInput(i, builtNode.GetOutput(dependency.OutputIndex));
				}
				m_BuiltNodes[node.NodePtr] = BuiltMaterialNode(node.NodePtr->GetOutputCount());
				BuiltMaterialNode& builtNode = m_BuiltNodes[node.NodePtr];
				node.NodePtr->Build(builtNode, inputs, GetContext(), *this);
				if (!wasCompatible)
				{
					VertexShader& vertex = GetBuilder().Factory().Vertex();
					FragmentShader& fragment = GetBuilder().Factory().Fragment();
					for (int i = 0; i < builtNode.GetOutputCount(); i++)
					{
						ShaderValuePtr value = builtNode.GetOutput(i);
						ShaderVariablePtr outValue = vertex.DeclarePassOut(value->Type());
						vertex.SetVariable(outValue, value);
						ShaderVariablePtr inValue = fragment.DeclarePassIn(outValue);
						builtNode.BuildOutput(i, inValue);
					}
				}
				m_CurrentProgram = oldCurrentProgram;
			}
		}
		return GetBuiltNode(&masterNode).GetOutput(0);
	}

	std::unique_ptr<Material> MaterialGraphBuilder::Build() const
	{
		return m_Builder.BuildMaterial();
	}

	void MaterialGraphBuilder::Reset()
	{
		m_Builder = MaterialBuilder();
		m_BuiltNodes.clear();
		m_CurrentProgram = &m_Builder.Factory().Vertex();
	}

	bool MaterialGraphBuilder::IsAlreadyBuilt(const MaterialNode* node) const
	{
		return m_BuiltNodes.find(node) != m_BuiltNodes.end();
	}

	const BuiltMaterialNode& MaterialGraphBuilder::GetBuiltNode(const MaterialNode* node) const
	{
		return m_BuiltNodes.at(node);
	}

	BuiltMaterialNode& MaterialGraphBuilder::GetBuiltNode(const MaterialNode* node)
	{
		return m_BuiltNodes.at(node);
	}

	std::vector<MaterialGraphBuilder::NodeInfo> MaterialGraphBuilder::FlattenNode(const MaterialNode& node)
	{
		std::vector<NodeInfo> result;
		FlattenNode(node, result);
		return result;
	}

	void MaterialGraphBuilder::FlattenNode(const MaterialNode& node, std::vector<MaterialGraphBuilder::NodeInfo>& current)
	{
		std::vector<NodeDependency> dependencies;
		for (int i = 0; i < node.GetInputCount(); i++)
		{
			const NodeConnection& connection = node.GetInputConnection(i);
			BLT_ASSERT(connection.IsValid(), "Invalid input connection");
			FlattenNode(*connection.GetNode(), current);
			dependencies.push_back({ connection.GetNode(), connection.GetOutputIndex() });
		}
		current.push_back({ &node, dependencies });
	}

}