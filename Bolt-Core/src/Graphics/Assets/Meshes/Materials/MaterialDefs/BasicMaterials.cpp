#include "bltpch.h"
#include "BasicMaterials.h"

namespace Bolt
{

	BasicMaterial::BasicMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{
	}

	BasicMaterial::BasicMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{
	}

	UniformLink<Color>& BasicMaterial::LinkColor(const Color& color)
	{
		return GetLinkContext().Link("Color", color);
	}

	UniformLink<std::function<Color()>>& BasicMaterial::LinkColor(const std::function<Color()>& color)
	{
		return GetLinkContext().Link("Color", color);
	}

	TextureBasicMaterial::TextureBasicMaterial(ShaderLinkContext&& shader, bool isTransparent) : Material(std::move(shader), isTransparent)
	{
	}

	TextureBasicMaterial::TextureBasicMaterial(const ShaderLinkContext& shader, bool isTransparent) : Material(shader, isTransparent)
	{
	}

	UniformLink<AssetHandle<Texture2D>>& TextureBasicMaterial::LinkTexture(const AssetHandle<Texture2D>& albedo)
	{
		return GetLinkContext().Link("Texture", albedo);
	}

	UniformLink<Matrix3f>& TextureBasicMaterial::LinkTextureTransform(const Matrix3f& transform)
	{
		return GetLinkContext().Link("TexCoordTransform", transform);
	}

	UniformLink<std::function<AssetHandle<Texture2D>()>>& TextureBasicMaterial::LinkColor(const std::function<AssetHandle<Texture2D>()>& albedo)
	{
		return GetLinkContext().Link("Texture", albedo);
	}

	UniformLink<std::function<Matrix3f()>>& TextureBasicMaterial::LinkTextureTransform(const std::function<Matrix3f()>& transform)
	{
		return GetLinkContext().Link("TexCoordTransform", transform);
	}

}
