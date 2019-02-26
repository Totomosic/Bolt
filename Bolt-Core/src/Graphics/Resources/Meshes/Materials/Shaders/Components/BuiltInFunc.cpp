#include "Types.h"
#include "BuiltInFunc.h"

namespace Bolt
{

	BuiltInFunc::BuiltInFunc()
		: m_Overloads()
	{
	
	}

	const std::vector<BuiltInFunc::Overload>& BuiltInFunc::Overloads() const
	{
		return m_Overloads;
	}

	bool BuiltInFunc::HasOverload(const std::vector<ValueType>& inputs, ValueType output) const
	{
		auto it = std::find_if(m_Overloads.begin(), m_Overloads.end(), [&inputs, output](const Overload& overload)
		{
			return overload.Output == output && overload.Inputs == inputs;
		});
		return it != m_Overloads.end();
	}

	void BuiltInFunc::AddOverload(const std::vector<ValueType>& inputs, ValueType output, const blt::string& source)
	{
		m_Overloads.push_back({ inputs, source, output });
	}

	BuiltInFunc::OverloadInstance BuiltInFunc::GetSource(const std::vector<ShaderValuePtr>& inputs) const
	{
		std::vector<ValueType> inputTypes;
		inputTypes.reserve(inputs.size());
		for (const ShaderValuePtr& ptr : inputs)
		{
			inputTypes.push_back(ptr->Type());
		}
		const Overload& o = GetOverload(inputTypes);
		const blt::string& templateSource = o.SourceTemplate;
		blt::string source = templateSource;
		for (int i = 0; i < inputs.size(); i++)
		{
			source.replace_all('p' + std::to_string(i), inputs.at(i)->VarName());
		}
		return { o.Output, source };
	}

	const BuiltInFunc::Overload& BuiltInFunc::GetOverload(const std::vector<ValueType>& inputs) const
	{
		auto it = std::find_if(m_Overloads.begin(), m_Overloads.end(), [&inputs](const Overload& overload)
		{
			return overload.Inputs == inputs;
		});
		BLT_ASSERT(it != m_Overloads.end(), "Unable to find overload");
		return *it;
	}

}