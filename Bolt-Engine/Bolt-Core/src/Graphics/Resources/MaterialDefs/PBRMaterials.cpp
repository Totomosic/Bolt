#include "bltpch.h"
#include "PBRMaterials.h"

namespace Bolt
{

	// ===================================================================================================================================================================
	// PBR MATERIAL
	// ===================================================================================================================================================================

	PBRMaterial::PBRMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{

	}

	PBRMaterial::PBRMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{

	}

	UniformLink<Color>& PBRMaterial::LinkAlbedo(const Color& albedo)
	{
		return GetLinkContext().Link("Albedo", albedo);
	}

	UniformLink<float>& PBRMaterial::LinkMetallic(float metallic)
	{
		return GetLinkContext().Link("Metallic", metallic);
	}

	UniformLink<float>& PBRMaterial::LinkRoughness(float roughness)
	{
		return GetLinkContext().Link("Roughness", roughness);
	}

	UniformLink<float>& PBRMaterial::LinkAO(float ao)
	{
		return GetLinkContext().Link("AO", ao);
	}

	UniformLink<float>& PBRMaterial::LinkAlpha(float alpha)
	{
		return GetLinkContext().Link("Alpha", alpha);
	}

	UniformLink<float>& PBRMaterial::LinkAlphaThreshold(float threshold)
	{
		return GetLinkContext().Link("AlphaThreshold", threshold);
	}

	UniformLink<std::function<Color()>>& PBRMaterial::LinkAlbedo(const std::function<Color()>& albedo)
	{
		return GetLinkContext().Link("Albedo", albedo);
	}

	UniformLink<std::function<float()>>& PBRMaterial::LinkMetallic(const std::function<float()>& metallic)
	{
		return GetLinkContext().Link("Metallic", metallic);
	}

	UniformLink<std::function<float()>>& PBRMaterial::LinkRoughness(const std::function<float()>& roughness)
	{
		return GetLinkContext().Link("Roughness", roughness);
	}

	UniformLink<std::function<float()>>& PBRMaterial::LinkAO(const std::function<float()>& ao)
	{
		return GetLinkContext().Link("AO", ao);
	}

	UniformLink<std::function<float()>>& PBRMaterial::LinkAlpha(const std::function<float()>& alpha)
	{
		return GetLinkContext().Link("Alpha", alpha);
	}

	UniformLink<std::function<float()>>& PBRMaterial::LinkAlphaThreshold(const std::function<float()>& threshold)
	{
		return GetLinkContext().Link("AlphaThreshold", threshold);
	}

	// ===================================================================================================================================================================
	// PBR TEXTURE MATERIAL
	// ===================================================================================================================================================================

	PBRTextureMaterial::PBRTextureMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{
	
	}

	PBRTextureMaterial::PBRTextureMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{
	
	}

	UniformLink<ResourcePtr<Texture2D>>& PBRTextureMaterial::LinkAlbedo(const ResourcePtr<Texture2D>& albedo)
	{
		return GetLinkContext().Link("Albedo", albedo);
	}

	UniformLink<ResourcePtr<Texture2D>>& PBRTextureMaterial::LinkMetallic(const ResourcePtr<Texture2D>& metallic)
	{
		return GetLinkContext().Link("Metallic", metallic);
	}

	UniformLink<ResourcePtr<Texture2D>>& PBRTextureMaterial::LinkRoughness(const ResourcePtr<Texture2D>& roughness)
	{
		return GetLinkContext().Link("Roughness", roughness);
	}

	UniformLink<ResourcePtr<Texture2D>>& PBRTextureMaterial::LinkAO(const ResourcePtr<Texture2D>& ao)
	{
		return GetLinkContext().Link("AO", ao);
	}

	UniformLink<float>& PBRTextureMaterial::LinkAlpha(float alpha)
	{
		return GetLinkContext().Link("Alpha", alpha);
	}

	UniformLink<float>& PBRTextureMaterial::LinkAlphaThreshold(float threshold)
	{
		return GetLinkContext().Link("AlphaThreshold", threshold);
	}

	UniformLink<std::function<ResourcePtr<Texture2D>()>>& PBRTextureMaterial::LinkAlbedo(const std::function<ResourcePtr<Texture2D>()>& albedo)
	{
		return GetLinkContext().Link("Albedo", albedo);
	}

	UniformLink<std::function<ResourcePtr<Texture2D>()>>& PBRTextureMaterial::LinkMetallic(const std::function<ResourcePtr<Texture2D>()>& metallic)
	{
		return GetLinkContext().Link("Metallic", metallic);
	}

	UniformLink<std::function<ResourcePtr<Texture2D>()>>& PBRTextureMaterial::LinkRoughness(const std::function<ResourcePtr<Texture2D>()>& roughness)
	{
		return GetLinkContext().Link("Roughness", roughness);
	}

	UniformLink<std::function<ResourcePtr<Texture2D>()>>& PBRTextureMaterial::LinkAO(const std::function<ResourcePtr<Texture2D>()>& ao)
	{
		return GetLinkContext().Link("AO", ao);
	}

	UniformLink<std::function<float()>>& PBRTextureMaterial::LinkAlpha(const std::function<float()>& alpha)
	{
		return GetLinkContext().Link("Alpha", alpha);
	}

	UniformLink<std::function<float()>>& PBRTextureMaterial::LinkAlphaThreshold(const std::function<float()>& threshold)
	{
		return GetLinkContext().Link("AlphaThreshold", threshold);
	}

}