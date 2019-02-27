#include "Types.h"
#include "FunctionContext.h"

namespace Bolt
{

	FunctionContext::FunctionContext(ShaderType shader)
		: m_Functions(), m_FunctionIndices(), m_Shader(shader)
	{
		
	}

	ShaderType FunctionContext::Shader() const
	{
		return m_Shader;
	}

	int FunctionContext::FunctionCount() const
	{
		return m_Functions.size();
	}

	bool FunctionContext::HasFunction(const blt::string& name) const
	{
		return m_FunctionIndices.find(name) != m_FunctionIndices.end();
	}

	bool FunctionContext::IsDefined(const blt::string& name) const
	{
		return GetFunctionImpl(name).IsDefined;
	}

	const FunctionDecl& FunctionContext::GetFunctionDecl(const blt::string& name) const
	{
		return GetFunctionImpl(name).Declaration;
	}
	
	const FunctionDef& FunctionContext::GetFunction(const blt::string& name) const
	{
		const auto& impl = GetFunctionImpl(name);
		BLT_ASSERT(impl.IsDefined, "Cannot get definition of undefined function");
		return impl.Definition;
	}

	const FunctionDecl& FunctionContext::AddFunction(const blt::string& name, const std::vector<ValueType>& inputs, ValueType outputs)
	{
		FunctionDecl decl(name, inputs, outputs, Shader());
		return AddFunction(decl);
	}

	const FunctionDecl& FunctionContext::AddFunction(const FunctionDecl& declaration)
	{
		size_t index = m_Functions.size();
		m_Functions.push_back({ declaration, FunctionDef(), false });
		m_FunctionIndices[declaration.Name()] = index;
		return m_Functions.at(index).Declaration;
	}

	void FunctionContext::AddFunctionDefinition(const FunctionDef& definition)
	{
		BLT_ASSERT(HasFunction(definition.Declaration().Name()), "No function to define");
		FunctionImpl& func = GetFunctionImpl(definition.Declaration().Name());
		func.Definition = definition;
		func.IsDefined = true;
	}

	blt::string FunctionContext::GetSource() const
	{
		blt::string source = "";
		if (m_Functions.size() > 0)
		{
			const auto& func = m_Functions.at(0);
			BLT_ASSERT(func.IsDefined, "Use of undefined Function");
			source += func.Definition.GetSource();
			for (int i = 1; i < m_Functions.size(); i++)
			{
				const auto& func = m_Functions.at(i);
				BLT_ASSERT(func.IsDefined, "Use of undefined Function");
				source += "\n\n" + func.Definition.GetSource();
			}
		}
		return source;
	}

	void FunctionContext::Reset()
	{
		m_Shader = ShaderType::Vertex;
		m_FunctionIndices.clear();
		m_Functions.clear();
	}

	const FunctionContext::FunctionImpl& FunctionContext::GetFunctionImpl(const blt::string& name) const
	{
		return m_Functions.at(m_FunctionIndices.at(name));
	}

	FunctionContext::FunctionImpl& FunctionContext::GetFunctionImpl(const blt::string& name)
	{
		return m_Functions.at(m_FunctionIndices.at(name));
	}

}