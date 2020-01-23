#include "bltpch.h"
#include "MaterialGraphBuilder.h"
#include "../Material.h"

namespace Bolt
{

	MaterialGraphBuilder::MaterialGraphBuilder()
		: m_Builder(), m_Context(), m_BuiltVertexNodes(), m_BuiltFragmentNodes(), m_CurrentProgram(nullptr)
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
			if (!IsAlreadyBuilt(node.NodePtr, m_CurrentProgram->Type()))
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
					BLT_ASSERT(IsAlreadyBuilt(dependency.NodePtr, oldCurrentProgram->Type()), "Invalid flatten order");
					const BuiltMaterialNode& builtNode = GetBuiltNode(dependency.NodePtr, oldCurrentProgram->Type());
					inputs.SetInput(i, builtNode.GetOutput(dependency.OutputIndex));
				}
				BuiltMaterialNode& builtNode = CreateNewBuiltNode(node.NodePtr, oldCurrentProgram->Type(), BuiltMaterialNode(node.NodePtr->GetOutputCount()));
				node.NodePtr->Build(builtNode, inputs, GetContext(), *this);
				if (!wasCompatible)
				{
					VertexShader& vertex = GetBuilder().Factory().Vertex();
					FragmentShader& fragment = GetBuilder().Factory().Fragment();
					for (int i = 0; i < builtNode.GetOutputCount(); i++)
					{
						ShaderValuePtr value = builtNode.GetOutput(i);
						ShaderPassVariablePtr outValue = vertex.DeclarePassOut(value->Type());
						vertex.SetVariable(outValue, value);
						ShaderPassVariablePtr inValue = fragment.DeclarePassIn(outValue);
						builtNode.BuildOutput(i, inValue);
					}
				}
				m_CurrentProgram = oldCurrentProgram;
			}
		}
		return GetBuiltNode(&masterNode, m_CurrentProgram->Type()).GetOutput(0);
	}

	void MaterialGraphBuilder::Reset()
	{
		m_Builder = MaterialBuilder();
		m_BuiltVertexNodes.clear();
		m_BuiltFragmentNodes.clear();
		m_CurrentProgram = &m_Builder.Factory().Vertex();
	}

	bool MaterialGraphBuilder::IsAlreadyBuilt(const MaterialNode* node, ShaderStage stage) const
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return m_BuiltVertexNodes.find(node) != m_BuiltVertexNodes.end();
		case ShaderStage::Fragment:
			return m_BuiltFragmentNodes.find(node) != m_BuiltFragmentNodes.end();
		default:
			break;
		}
		BLT_ASSERT(false, "Invalid shader stage");
		return false;
	}

	const BuiltMaterialNode& MaterialGraphBuilder::GetBuiltNode(const MaterialNode* node, ShaderStage stage) const
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return m_BuiltVertexNodes.at(node);
		case ShaderStage::Fragment:
			return m_BuiltFragmentNodes.at(node);
		default:
			break;
		}
		BLT_ASSERT(false, "Invalid shader stage");
		return m_BuiltVertexNodes.at(node);
	}

	BuiltMaterialNode& MaterialGraphBuilder::GetBuiltNode(const MaterialNode* node, ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return m_BuiltVertexNodes.at(node);
		case ShaderStage::Fragment:
			return m_BuiltFragmentNodes.at(node);
		default:
			break;
		}
		BLT_ASSERT(false, "Invalid shader stage");
		return m_BuiltVertexNodes.at(node);
	}

	BuiltMaterialNode& MaterialGraphBuilder::CreateNewBuiltNode(const MaterialNode* node, ShaderStage stage, BuiltMaterialNode&& builtNode)
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			m_BuiltVertexNodes[node] = std::move(builtNode);
			return m_BuiltVertexNodes.at(node);
		case ShaderStage::Fragment:
			m_BuiltFragmentNodes[node] = std::move(builtNode);
			return m_BuiltFragmentNodes.at(node);
		default:
			break;
		}
		BLT_ASSERT(false, "Invalid shader stage");
		return m_BuiltVertexNodes.at(node);
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