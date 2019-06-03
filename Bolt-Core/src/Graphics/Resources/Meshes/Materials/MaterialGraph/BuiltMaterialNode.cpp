#include "bltpch.h"
#include "BuiltMaterialNode.h"

namespace Bolt
{

	BuiltMaterialNode::BuiltMaterialNode() : BuiltMaterialNode(0)
	{
	
	}

	BuiltMaterialNode::BuiltMaterialNode(int outputCount)
		: m_OutputCount(outputCount), m_BuiltOutputs(nullptr)
	{
		if (outputCount > 0)
		{
			m_BuiltOutputs = std::make_unique<ShaderValuePtr[]>(m_OutputCount);
		}
	}

	int BuiltMaterialNode::GetOutputCount() const
	{
		return m_OutputCount;
	}

	const ShaderValuePtr& BuiltMaterialNode::GetOutput(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetOutputCount(), "Invalid output index");
		return m_BuiltOutputs[index];
	}

	bool BuiltMaterialNode::IsBuilt(int index) const
	{
		return GetOutput(index) != nullptr;
	}

	void BuiltMaterialNode::BuildOutput(int index, const ShaderValuePtr& value)
	{
		BLT_ASSERT(index >= 0 && index < GetOutputCount(), "Invalid output index");
		m_BuiltOutputs[index] = value;
	}


	LinkedInputs::LinkedInputs(int inputCount)
		: m_InputCount(inputCount), m_Inputs(nullptr)
	{
		if (m_InputCount > 0)
		{
			m_Inputs = std::make_unique<ShaderValuePtr[]>(m_InputCount);
		}
	}

	LinkedInputs::LinkedInputs(const std::vector<ShaderValuePtr>& inputs) : LinkedInputs(inputs.size())
	{
		for (int i = 0; i < (int)inputs.size(); i++)
		{
			SetInput(i, inputs.at(i));
		}
	}

	int LinkedInputs::GetInputCount() const
	{
		return m_InputCount;
	}

	const ShaderValuePtr& LinkedInputs::GetInput(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetInputCount(), "Invalid input index");
		return m_Inputs[index];
	}

	void LinkedInputs::SetInput(int index, const ShaderValuePtr& input)
	{
		BLT_ASSERT(index >= 0 && index < GetInputCount(), "Invalid input index");
		m_Inputs[index] = input;
	}

}