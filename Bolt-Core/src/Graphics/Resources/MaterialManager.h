#pragma once
#include "Meshes/Materials/MaterialBuilder.h"
#include "Meshes/Materials/Material.h"
#include "Textures/Fonts/Font.h"

namespace Bolt
{

	class BLT_API MaterialManager
	{
	private:
		MaterialBuilder m_DefaultBuilder;
		MaterialBuilder m_TextureBuilder;
		MaterialBuilder m_FontBuilder;
		MaterialBuilder m_DefaultLightingBuilder;
		MaterialBuilder m_PBRBuilder;
		MaterialBuilder m_PBRTextureBuilder;
		
	public:
		MaterialManager();

		std::unique_ptr<Material> Default(const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> Texture(const ResourcePtr<Texture2D>& texture) const;
		std::unique_ptr<Material> Font(const ResourcePtr<Bolt::Font>& font, const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> DefaultLighting(const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> PBR() const;
		std::unique_ptr<Material> PBRTexture() const;

	private:
		void CreateDefaultMaterial(MaterialBuilder& builder) const;
		void CreateTextureMaterial(MaterialBuilder& builder) const;
		void CreateFontMaterial(MaterialBuilder& builder) const;
		
		void CreateDefaultLightingMaterial(MaterialBuilder& builder) const;

		void CreatePBRMaterial(MaterialBuilder& builder) const;
		void CreatePBRTextureMaterial(MaterialBuilder& builder) const;

	};

}