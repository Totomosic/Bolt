#pragma once
#include "../Material.h"

namespace Bolt
{

	class BLT_API DefaultLightingMaterial : public Material
	{
	public:
		DefaultLightingMaterial(ShaderLinkContext&& shader, bool isTransparent);
		DefaultLightingMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<Color>& LinkColor(const Color& albedoRGB);
		UniformLink<Color>& LinkSpecularColor(const Color& specularRGB);
		UniformLink<float>& LinkShininess(float shininess);
		UniformLink<float>& LinkShineDamper(float shineDamper);

		UniformLink<std::function<Color()>>& LinkColor(const std::function<Color()>& albedoRGB);
		UniformLink<std::function<Color()>>& LinkSpecularColor(const std::function<Color()>& specularRGB);
		UniformLink<std::function<float()>>& LinkShininess(const std::function<float()>& shininess);
		UniformLink<std::function<float()>>& LinkShineDamper(const std::function<float()>& shineDamper);
	};

	class BLT_API TextureLightingMaterial : public Material
	{
	public:
		TextureLightingMaterial(ShaderLinkContext&& shader, bool isTransparent);
		TextureLightingMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<ResourcePtr<Texture2D>>& LinkColor(const ResourcePtr<Texture2D>& albedo);
		UniformLink<Color>& LinkSpecularColor(const Color& specularRGB);
		UniformLink<float>& LinkShininess(float shininess);
		UniformLink<float>& LinkShineDamper(float shineDamper);

		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkColor(const std::function<ResourcePtr<Texture2D>()>& albedo);
		UniformLink<std::function<Color()>>& LinkSpecularColor(const std::function<Color()>& specularRGB);
		UniformLink<std::function<float()>>& LinkShininess(const std::function<float()>& shininess);
		UniformLink<std::function<float()>>& LinkShineDamper(const std::function<float()>& shineDamper);

	};

}