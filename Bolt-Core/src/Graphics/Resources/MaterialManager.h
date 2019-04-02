#pragma once
#include "Meshes/Materials/__Materials__.h"
#include "Textures/Fonts/Font.h"

namespace Bolt
{

	class BLT_API MaterialManager
	{
	private:
		MaterialBuilder m_DefaultBuilder;
		MaterialBuilder m_TextureBuilder;
		MaterialBuilder m_FontBuilder;
		
	public:
		MaterialManager();

		std::unique_ptr<Material> Default(const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> Texture(const ResourcePtr<const Texture2D>& texture) const;
		std::unique_ptr<Material> Font(const ResourcePtr<const Bolt::Font>& font, const Color& baseColor = Color::White) const;

	private:
		void CreateDefaultMaterial(MaterialBuilder& builder) const;
		void CreateTextureMaterial(MaterialBuilder& builder) const;
		void CreateFontMaterial(MaterialBuilder& builder) const;

	};

}