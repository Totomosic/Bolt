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

	const ShaderLinkContext& Material::GetLinkContext() const
	{
		return m_Shader;
	}

	ShaderLinkContext& Material::GetLinkContext()
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

}