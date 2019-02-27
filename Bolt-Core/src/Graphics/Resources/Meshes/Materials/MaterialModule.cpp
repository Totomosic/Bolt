#include "Types.h"
#include "MaterialModule.h"

namespace Bolt
{

	MaterialModule::MaterialModule(ShaderType shader)
		: m_Shader(shader)
	{
		
	}

	ShaderType MaterialModule::GetShader() const
	{
		return m_Shader;
	}

	ShaderValuePtr MaterialModule::GetValue(ShaderFactory& factory, ShaderValuePtr value) const
	{
		BLT_ASSERT((int)value->Shader() <= (int)m_Shader || value->Shader() == ShaderType::Ignore, "Cannot pass value backwards");
		if (value->Shader() == m_Shader || value->Shader() == ShaderType::Ignore)
		{
			return std::move(value);
		}
		ShaderValuePtr passedValue = factory.Pass(value);
		return GetValue(factory, passedValue);
	}

}