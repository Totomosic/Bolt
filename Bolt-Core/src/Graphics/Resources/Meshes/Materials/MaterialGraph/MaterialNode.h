#pragma once
#include "InputPort.h"
#include "OutputPort.h"
#include "NodeConnection.h"
#include "BuiltMaterialNode.h"

#include "../Shaders/Components/ShaderFuncs.h"
#include "../Shaders/Components/Values/ShaderLiteral.h"

namespace Bolt
{

	class MaterialGraphBuilder;
	class MaterialGraphContext;
	class MaterialGraph;

	class BLT_API MaterialNode
	{
	public:
		struct BLT_API InputLink
		{
		public:
			InputPort Port;
			NodeConnection Connection;
		};

	private:
		ShaderStageCompatibility m_Compatibility;
		int m_InputCount;
		int m_OutputCount;
		std::unique_ptr<InputLink[]> m_Inputs;
		std::unique_ptr<OutputPort[]> m_Outputs;

	public:
		MaterialNode(ShaderStageCompatibility compatibility, int inputs, int outputs);
		virtual ~MaterialNode() {}

		ShaderStageCompatibility GetCompatibility() const;
		int GetInputCount() const;
		int GetOutputCount() const;
		const InputPort& GetInputPort(int index) const;
		const OutputPort& GetOutputPort(int index) const;
		const NodeConnection& GetInputConnection(int index) const;

		void Connect(int inputIndex, const NodeConnection& connection);
		NodeConnection GetConnection(int outputIndex) const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const = 0;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) = 0;

	protected:
		void SetInput(int index, const InputPort& port);
		void SetOutput(int index, const OutputPort& port);

	};

}