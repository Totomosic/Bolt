#pragma once
#include "..\Texture2D.h"

namespace Bolt
{

	class BLT_API Font : public Texture2D
	{
	public:
		struct BLT_API FontCharacter
		{
		public:
			TextureBounds Bounds;
			size_t Width;
			size_t Height;
			float AdvanceX;
			float AdvanceY;
			float OffsetX;
			float OffsetY;
			float Kerning;
		};

	private:
		float m_FontSize;
		std::unique_ptr<texture_atlas_t, std::function<void(texture_atlas_t*)>> m_TextureAtlas;
		std::unique_ptr<texture_font_t, std::function<void(texture_font_t*)>> m_TextureFont;

	public:
		Font(const Filepath& fontFile, float fontSize, int textureWidth = 512, int textureHeight = 512);
		Font(const byte* data, uint size, float fontSize, int textureWidth = 512, int textureHeight = 512);

		float FontSize() const;
		std::vector<FontCharacter> GetCharacters(const blt::string& str) const;
		Vector2f SizeOfText(const blt::string& text) const;

	};

}