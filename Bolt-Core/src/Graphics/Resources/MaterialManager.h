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
		
	public:
		MaterialManager();

		std::unique_ptr<Material> Default(const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> Texture(const ResourcePtr<const Texture2D>& texture) const;
		std::unique_ptr<Material> Font(const ResourcePtr<const Bolt::Font>& font, const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> DefaultLighting(const Color& baseColor = Color::White) const;

	private:
		void CreateDefaultMaterial(MaterialBuilder& builder) const;
		void CreateTextureMaterial(MaterialBuilder& builder) const;
		void CreateFontMaterial(MaterialBuilder& builder) const;
		
		void CreateDefaultLightingMaterial(MaterialBuilder& builder) const;

	};

}