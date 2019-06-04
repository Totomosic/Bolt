#pragma once
#include "../MaterialBuilder.h"
#include "MaterialGraphContext.h"
#include "Nodes/MasterNode.h"

namespace Bolt
{

	class BLT_API MaterialGraphBuilder
	{
	public:
		struct BLT_API NodeDependency
		{
		public:
			const MaterialNode* NodePtr;
			int OutputIndex;
		};

		struct BLT_API NodeInfo
		{
		public:
			const MaterialNode* NodePtr;
			std::vector<NodeDependency> Dependencies;
		};

	private:
		MaterialBuilder m_Builder;
		MaterialGraphContext m_Context;

		std::unordered_map<const MaterialNode*, BuiltMaterialNode> m_BuiltVertexNodes;
		std::unordered_map<const MaterialNode*, BuiltMaterialNode> m_BuiltFragmentNodes;
		ShaderProgram* m_CurrentProgram;

	public:
		MaterialGraphBuilder();

		const MaterialGraphContext& GetContext() const;
		ShaderProgram& GetProgram() const;
		MaterialBuilder& GetBuilder();

		ShaderValuePtr BuildNode(const MasterNode& node);
		std::unique_ptr<Material> Build() const;
		void Reset();

	private:
		bool IsAlreadyBuilt(const MaterialNode* node, ShaderStage stage) const;
		const BuiltMaterialNode& GetBuiltNode(const MaterialNode* node, ShaderStage stage) const;
		BuiltMaterialNode& GetBuiltNode(const MaterialNode* node, ShaderStage stage);
		BuiltMaterialNode& CreateNewBuiltNode(const MaterialNode* node, ShaderStage stage, BuiltMaterialNode&& builtNode);

	private:
		static std::vector<NodeInfo> FlattenNode(const MaterialNode& node);
		static void FlattenNode(const MaterialNode& node, std::vector<NodeInfo>& current);

	};

}