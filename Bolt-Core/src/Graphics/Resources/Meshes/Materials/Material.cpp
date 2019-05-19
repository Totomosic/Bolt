#include "bltpch.h"
#include "Material.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	Material::Material(ShaderLinkContext&& shader, bool isTransparent)
		: m_Shader(std::move(shader)), m_RenderSettings()
	{
		SetIsTransparent(isTransparent);
	}

	Material::Material(const ShaderLinkContext& shader, bool isTransparent)
		: m_Shader(shader), m_RenderSettings()
	{
		SetIsTransparent(isTransparent);
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

	bool Material::IsTransparent() const
	{
		return m_RenderSettings.UseBlend;
	}

	void Material::SetIsTransparent(bool isTransparent)
	{
		m_RenderSettings.UseBlend = isTransparent;
	}

	std::unique_ptr<Material> Material::Clone() const
	{
		std::unique_ptr<Material> material = std::make_unique<Material>(*this);
		return material;
	}

}