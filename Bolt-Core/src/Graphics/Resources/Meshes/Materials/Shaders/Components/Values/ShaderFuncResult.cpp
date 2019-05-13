#include "Types.h"
#include "ShaderFuncResult.h"
#include "../ShaderBuilder.h"

namespace Bolt
{
	
	ShaderFuncResult::ShaderFuncResult(const blt::string& templateString, const std::vector<ShaderValuePtr>& inputs, ValueType resultType, ValueTypeDim dim) : ShaderValue(resultType, dim),
		m_TemplateString(templateString), m_Inputs(inputs)
	{
		
	}

	void ShaderFuncResult::Build(ShaderBuilder& builder) const
	{
		uint32_t lastIndex = 0;
		uint32_t index = m_TemplateString.find_first_of(INPUT_TEMPLATE);
		while (index != blt::string::npos)
		{
			int paramIndex = std::stoi(m_TemplateString.substr(index + 1, 1).cpp_str());
			BLT_ASSERT(paramIndex < m_Inputs.size(), "Index out of range");
			blt::string prev = m_TemplateString.substr(lastIndex, index - lastIndex);
			builder.Write(prev);
			m_Inputs.at(paramIndex)->Build(builder);
			lastIndex = index + 2;
			index = m_TemplateString.find_first_of(INPUT_TEMPLATE, lastIndex + 1);
		}
		builder.Write(m_TemplateString.substr(lastIndex));
	}

}