#pragma once
#include "../Material.h"

namespace Bolt
{

	class BLT_API PBRMaterial : public Material
	{
	public:
		PBRMaterial(ShaderLinkContext&& shader, bool isTransparent);
		PBRMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<Color>& LinkAlbedo(const Color& albedoRGB);
		UniformLink<float>& LinkMetallic(float metallic);
		UniformLink<float>& LinkRoughness(float roughness);
		UniformLink<float>& LinkAO(float ao);
		UniformLink<Vector3f>& LinkNormal(const Vector3f& normal);
		UniformLink<float>& LinkAlpha(float alpha);
		UniformLink<float>& LinkAlphaThreshold(float threshold);

		UniformLink<std::function<Color()>>& LinkAlbedo(const std::function<Color()>& albedoRGB);
		UniformLink<std::function<float()>>& LinkMetallic(const std::function<float()>& metallic);
		UniformLink<std::function<float()>>& LinkRoughness(const std::function<float()>& roughness);
		UniformLink<std::function<float()>>& LinkAO(const std::function<float()>& ao);
		UniformLink<std::function<Vector3f()>>& LinkNormal(const std::function<Vector3f()>& normal);
		UniformLink<std::function<float()>>& LinkAlpha(const std::function<float()>& alpha);
		UniformLink<std::function<float()>>& LinkAlphaThreshold(const std::function<float()>& threshold);
	};

	class BLT_API PBRTextureMaterial : public Material
	{
	public:
		PBRTextureMaterial(ShaderLinkContext&& shader, bool isTransparent);
		PBRTextureMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<ResourcePtr<Texture2D>>& LinkAlbedo(const ResourcePtr<Texture2D>& albedo);
		UniformLink<ResourcePtr<Texture2D>>& LinkMetallic(const ResourcePtr<Texture2D>& metallic);
		UniformLink<ResourcePtr<Texture2D>>& LinkRoughness(const ResourcePtr<Texture2D>& roughness);
		UniformLink<ResourcePtr<Texture2D>>& LinkAO(const ResourcePtr<Texture2D>& ao);
		UniformLink<ResourcePtr<Texture2D>>& LinkNormal(const ResourcePtr<Texture2D>& normal);
		UniformLink<float>& LinkAlpha(float alpha);
		UniformLink<float>& LinkAlphaThreshold(float threshold);

		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkAlbedo(const std::function<ResourcePtr<Texture2D>()>& albedo);
		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkMetallic(const std::function<ResourcePtr<Texture2D>()>& metallic);
		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkRoughness(const std::function<ResourcePtr<Texture2D>()>& roughness);
		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkAO(const std::function<ResourcePtr<Texture2D>()>& ao);
		UniformLink<std::function<ResourcePtr<Texture2D>()>>& LinkNormal(const std::function<ResourcePtr<Texture2D>()>& normal);
		UniformLink<std::function<float()>>& LinkAlpha(const std::function<float()>& alpha);
		UniformLink<std::function<float()>>& LinkAlphaThreshold(const std::function<float()>& threshold);

	};

}