#pragma once
#include "../Material.h"

namespace Bolt
{

	class BLT_API BasicMaterial : public Material
	{
	public:
		BasicMaterial(ShaderLinkContext&& shader, bool isTransparent);
		BasicMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<Color>& LinkColor(const Color& color);
		UniformLink<std::function<Color()>>& LinkColor(const std::function<Color()>& color);
	};

	class BLT_API TextureBasicMaterial : public Material
	{
	public:
		TextureBasicMaterial(ShaderLinkContext&& shader, bool isTransparent);
		TextureBasicMaterial(const ShaderLinkContext& shader, bool isTransparent);

		UniformLink<AssetHandle<Texture2D>>& LinkTexture(const AssetHandle<Texture2D>& albedo);
		UniformLink<Matrix3f>& LinkTextureTransform(const Matrix3f& transform);

		UniformLink<std::function<AssetHandle<Texture2D>()>>& LinkColor(const std::function<AssetHandle<Texture2D>()>& albedo);
		UniformLink<std::function<Matrix3f()>>& LinkTextureTransform(const std::function<Matrix3f()>& transform);
	};

}