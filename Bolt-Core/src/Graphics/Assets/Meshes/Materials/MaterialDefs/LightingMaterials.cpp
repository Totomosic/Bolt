#include "bltpch.h"
#include "LightingMaterials.h"

namespace Bolt
{

	// ===================================================================================================================================================================
	// DEFAULT LIGHTING MATERIAL
	// ===================================================================================================================================================================

	DefaultLightingMaterial::DefaultLightingMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{
	}

	DefaultLightingMaterial::DefaultLightingMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{
	}

	UniformLink<Color>& DefaultLightingMaterial::LinkColor(const Color& abledoRGB)
	{
		return GetLinkContext().Link("Color", abledoRGB);
	}

	UniformLink<Color>& DefaultLightingMaterial::LinkSpecularColor(const Color& specularRGB)
	{
		return GetLinkContext().Link("SpecularColor", specularRGB);
	}

	UniformLink<float>& DefaultLightingMaterial::LinkShininess(float shininess)
	{
		return GetLinkContext().Link("Shininess", shininess);
	}

	UniformLink<float>& DefaultLightingMaterial::LinkShineDamper(float shineDamper)
	{
		return GetLinkContext().Link("ShineDamper", shineDamper);
	}

	UniformLink<std::function<Color()>>& DefaultLightingMaterial::LinkColor(const std::function<Color()>& abledoRGB)
	{
		return GetLinkContext().Link("Color", abledoRGB);
	}

	UniformLink<std::function<Color()>>& DefaultLightingMaterial::LinkSpecularColor(const std::function<Color()>& specularRGB)
	{
		return GetLinkContext().Link("SpecularColor", specularRGB);
	}

	UniformLink<std::function<float()>>& DefaultLightingMaterial::LinkShininess(const std::function<float()>& shininess)
	{
		return GetLinkContext().Link("Shininess", shininess);
	}

	UniformLink<std::function<float()>>& DefaultLightingMaterial::LinkShineDamper(const std::function<float()>& shineDamper)
	{
		return GetLinkContext().Link("ShineDamper", shineDamper);
	}

	// ===================================================================================================================================================================
	// TEXTURE LIGHTING MATERIAL
	// ===================================================================================================================================================================

	TextureLightingMaterial::TextureLightingMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{
	}

	TextureLightingMaterial::TextureLightingMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{
	}

	UniformLink<AssetHandle<Texture2D>>& TextureLightingMaterial::LinkTexture(const AssetHandle<Texture2D>& albedo)
	{
		return GetLinkContext().Link("Texture", albedo);
	}

	UniformLink<Color>& TextureLightingMaterial::LinkSpecularColor(const Color& specularRGB)
	{
		return GetLinkContext().Link("SpecularColor", specularRGB);
	}

	UniformLink<float>& TextureLightingMaterial::LinkShininess(float shininess)
	{
		return GetLinkContext().Link("Shininess", shininess);
	}

	UniformLink<float>& TextureLightingMaterial::LinkShineDamper(float shineDamper)
	{
		return GetLinkContext().Link("ShineDamper", shineDamper);
	}

	UniformLink<std::function<AssetHandle<Texture2D>()>>& TextureLightingMaterial::LinkColor(const std::function<AssetHandle<Texture2D>()>& albedo)
	{
		return GetLinkContext().Link("Color", albedo);
	}

	UniformLink<std::function<Color()>>& TextureLightingMaterial::LinkSpecularColor(const std::function<Color()>& specularRGB)
	{
		return GetLinkContext().Link("SpecularColor", specularRGB);
	}

	UniformLink<std::function<float()>>& TextureLightingMaterial::LinkShininess(const std::function<float()>& shininess)
	{
		return GetLinkContext().Link("Shininess", shininess);
	}

	UniformLink<std::function<float()>>& TextureLightingMaterial::LinkShineDamper(const std::function<float()>& shineDamper)
	{
		return GetLinkContext().Link("ShineDamper", shineDamper);
	}

}