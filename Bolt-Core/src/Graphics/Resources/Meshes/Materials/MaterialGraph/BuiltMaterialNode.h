#pragma once
#include "../Shaders/Components/Values/ShaderValue.h"

namespace Bolt
{

	class BLT_API BuiltMaterialNode
	{
	private:
		int m_OutputCount;
		std::unique_ptr<ShaderValuePtr[]> m_BuiltOutputs;

	public:
		BuiltMaterialNode();
		BuiltMaterialNode(int outputCount);

		int GetOutputCount() const;
		const ShaderValuePtr& GetOutput(int index) const;
		bool IsBuilt(int index) const;

		void BuildOutput(int index, const ShaderValuePtr& value);

	};

	class BLT_API LinkedInputs
	{
	private:
		int m_InputCount;
		std::unique_ptr<ShaderValuePtr[]> m_Inputs;

	public:
		LinkedInputs(int inputCount);
		LinkedInputs(const std::vector<ShaderValuePtr>& inputs);

		int GetInputCount() const;
		const ShaderValuePtr& GetInput(int index) const;
		
		void SetInput(int index, const ShaderValuePtr& input);
	};

}