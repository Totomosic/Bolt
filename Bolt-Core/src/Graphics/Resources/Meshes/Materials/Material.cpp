#include "Types.h"
#include "Material.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	Material::Material(ShaderLinkContext&& shader)
		: m_Builder(nullptr), m_Shader(std::move(shader))
	{
		
	}

	const ShaderLinkContext& Material::GetShader() const
	{
		return m_Shader;
	}

	ShaderLinkContext& Material::GetShader()
	{
		return m_Shader;
	}

	std::unique_ptr<Material> Material::Clone() const
	{
		return m_Builder->BuildMaterial();
	}

	void Material::SetBuilder(const MaterialBuilder* builder)
	{
		m_Builder = builder;
	}

}