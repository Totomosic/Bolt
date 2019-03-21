#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_Type(type)
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_Type;
	}

	ShaderPassValuePtr ShaderProgram::Pass(ShaderValuePtr value)
	{
		ShaderPassValuePtr pass = std::make_shared<ShaderPassValue>(std::move(value));
		m_PassValues.push_back(pass);
		return std::move(pass);
	}

	void ShaderProgram::Reset()
	{
		
	}

}