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

	void ShaderProgram::Reset()
	{
		
	}

}