#pragma once
#include "../Texture2D.h"
#include "Core/Utils/Filesystem/Filesystem.h"
#include "BoltLib/Math/Maths.h"

namespace Bolt
{

	class BLT_API Font : public Texture2D
	{
	public:
		struct BLT_API FontCharacter
		{
		public:
			TextureBounds Bounds;
			uint32_t Width;
			uint32_t Height;
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
		Font(const FilePath& fontFile, float fontSize, int textureWidth = 512, int textureHeight = 512);

		float FontSize() const;
		std::vector<FontCharacter> GetCharacters(const std::string& str) const;
		Vector2f SizeOfText(const std::string& text) const;

	};

}