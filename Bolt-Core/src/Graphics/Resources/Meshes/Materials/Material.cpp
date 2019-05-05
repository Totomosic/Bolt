#include "Types.h"
#include "Material.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	Material::Material(const MaterialBuilder& builder, ShaderLinkContext&& shader)
		: m_Builder(builder), m_Shader(std::move(shader)), m_RenderSettings()
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

	MaterialBuilder& Material::GetBuilder()
	{
		return m_Builder;
	}

	void Material::Rebuild()
	{
		std::unique_ptr<Material> newMaterial = m_Builder.BuildMaterial();
		ShaderLinkContext& links = newMaterial->GetShader();
		GetShader().CopyLinksTo(links);
		m_Shader = std::move(links);
	}

	std::unique_ptr<Material> Material::Clone() const
	{
		std::unique_ptr<Material> material = m_Builder.BuildMaterial();
		GetShader().CopyLinksTo(material->GetShader());
		return material;
	}

}