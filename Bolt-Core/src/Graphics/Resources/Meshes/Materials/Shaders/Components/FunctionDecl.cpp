#include "Types.h"
#include "FunctionDecl.h"

namespace Bolt
{

	FunctionDecl::FunctionDecl()
	{
	
	}

	FunctionDecl::FunctionDecl(const blt::string& name, const std::vector<ValueType>& inputs, ValueType output, ShaderType shader)
		: m_Name(name), m_Inputs(inputs), m_Output(output), m_Shader(shader)
	{
	
	}

	const blt::string& FunctionDecl::Name() const
	{
		return m_Name;
	}

	const std::vector<ValueType>& FunctionDecl::InputTypes() const
	{
		return m_Inputs;
	}

	ValueType FunctionDecl::OutputType() const
	{
		return m_Output;
	}

	ShaderType FunctionDecl::Shader() const
	{
		return m_Shader;
	}

}