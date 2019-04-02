#include "Types.h"
#include "Material.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	Material::Material(ShaderLinkContext&& shader)
		: m_Builder(nullptr), m_Shader(std::move(shader)), m_RenderSettings()
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

	const RenderSettings& Material::GetRenderSettings() const
	{
		return m_RenderSettings;
	}

	RenderSettings& Material::GetRenderSettings()
	{
		return m_RenderSettings;
	}

	std::unique_ptr<Material> Material::Clone() const
	{
		std::unique_ptr<Material> material = m_Builder->BuildMaterial();
		GetShader().CopyLinksTo(material->GetShader());
		return material;
	}

	void Material::SetBuilder(const MaterialBuilder* builder)
	{
		m_Builder = builder;
	}

}