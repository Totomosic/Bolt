#pragma once
#include "../Meshes/Materials/Material.h"

namespace Bolt
{

	class BLT_API PBRMaterial : public Material
	{
	public:
		PBRMaterial(ShaderLinkContext&& shader, bool isTransparent);
		PBRMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<Color>& LinkAlbedo(const Color& abledoRGB);
		UniformLink<float>& LinkMetallic(float metallic);
		UniformLink<float>& LinkRoughness(float roughness);
		UniformLink<float>& LinkAO(float ao);
		UniformLink<float>& LinkAlpha(float alpha);
		UniformLink<float>& LinkAlphaThreshold(float threshold);
	};

	class BLT_API PBRTextureMaterial : public Material
	{
	public:
		PBRTextureMaterial(ShaderLinkContext&& shader, bool isTransparent);
		PBRTextureMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<ResourcePtr<Texture2D>>& LinkAlbedo(const ResourcePtr<Texture2D>& abledo);
		UniformLink<ResourcePtr<Texture2D>>& LinkMetallic(const ResourcePtr<Texture2D>& metallic);
		UniformLink<ResourcePtr<Texture2D>>& LinkRoughness(const ResourcePtr<Texture2D>& roughness);
		UniformLink<ResourcePtr<Texture2D>>& LinkAO(const ResourcePtr<Texture2D>& ao);
		UniformLink<float>& LinkAlpha(float alpha);
		UniformLink<float>& LinkAlphaThreshold(float threshold);

	};

}