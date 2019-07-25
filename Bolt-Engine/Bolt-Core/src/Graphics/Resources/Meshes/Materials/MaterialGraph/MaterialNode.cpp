#include "bltpch.h"
#include "MaterialNode.h"

namespace Bolt
{

	MaterialNode::MaterialNode(ShaderStageCompatibility compatibility, int inputs, int outputs)
		: m_Compatibility(compatibility), m_InputCount(inputs), m_OutputCount(outputs), m_Inputs(nullptr), m_Outputs(nullptr)
	{
		if (m_InputCount > 0)
		{
			m_Inputs = std::make_unique<InputLink[]>(m_InputCount);
		}
		if (m_OutputCount > 0)
		{
			m_Outputs = std::make_unique<OutputPort[]>(m_OutputCount);
		}
	}

	ShaderStageCompatibility MaterialNode::GetCompatibility() const
	{
		return m_Compatibility;
	}

	int MaterialNode::GetInputCount() const
	{
		return m_InputCount;
	}

	int MaterialNode::GetOutputCount() const
	{
		return m_OutputCount;
	}

	const InputPort& MaterialNode::GetInputPort(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetInputCount(), "Invalid input index");
		return m_Inputs[index].Port;
	}

	const OutputPort& MaterialNode::GetOutputPort(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetOutputCount(), "Invalid output index");
		return m_Outputs[index];
	}

	const NodeConnection& MaterialNode::GetInputConnection(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetInputCount(), "Invalid connection index");
		return m_Inputs[index].Connection;
	}

	void MaterialNode::Connect(int inputIndex, const NodeConnection& connection)
	{
		BLT_ASSERT(inputIndex >= 0 && inputIndex < GetInputCount(), "Invalid input index");
		m_Inputs[inputIndex].Connection = connection;
	}

	NodeConnection MaterialNode::GetConnection(int outputIndex) const
	{
		BLT_ASSERT(outputIndex >= 0 && outputIndex < GetOutputCount(), "Invalid output index");
		m_Outputs[outputIndex].SetIsUsed(true);
		NodeConnection connection(this, outputIndex);
		return connection;
	}

	void MaterialNode::SetInput(int index, const InputPort& port)
	{
		BLT_ASSERT(index >= 0 && index < GetInputCount(), "Invalid input index");
		m_Inputs[index].Port = port;
	}

	void MaterialNode::SetOutput(int index, const OutputPort& port)
	{
		BLT_ASSERT(index >= 0 && index < GetOutputCount(), "Invalid output index");
		m_Outputs[index] = port;
	}

}