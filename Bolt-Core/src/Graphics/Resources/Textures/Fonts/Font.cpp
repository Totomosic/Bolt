#include "Font.h"

namespace Bolt
{

	Font::Font(const Filepath& fontFile, float fontSize, int textureWidth, int textureHeight) : Texture2D(textureWidth, textureHeight, TextureFormat::R, { WrapMode::ClampToEdge, MagFilter::Linear, MinFilter::Linear, Mipmaps::Disabled }),
		m_FontSize(fontSize)
	{
		m_TextureAtlas = std::unique_ptr<texture_atlas_t, std::function<void(texture_atlas_t*)>>(texture_atlas_new(m_Width, m_Height, 1), [](texture_atlas_t* ptr) { texture_atlas_delete(ptr); });
		m_TextureFont = std::unique_ptr<texture_font_t, std::function<void(texture_font_t*)>>(texture_font_new_from_file(m_TextureAtlas.get(), m_FontSize, fontFile.Path().c_str()), [](texture_font_t* ptr) { texture_font_delete(ptr); });
		GL_CALL(glDeleteTextures(1, &m_Id));
		GL_CALL(glGenTextures(1, &m_TextureAtlas->id));
		m_Id = m_TextureAtlas->id;
	}

	float Font::FontSize() const
	{
		return m_FontSize;
	}

	std::vector<Font::FontCharacter> Font::GetCharacters(const blt::string& str) const
	{
		std::vector<FontCharacter> result;
		for (int i = 0; i < str.size(); i++)
		{
			char c = str[i];
			texture_glyph_t* glyph = texture_font_get_glyph(m_TextureFont.get(), c);
			float kerning = 0;
			if (i != 0)
			{
				kerning = texture_glyph_get_kerning(glyph, str[i - 1]);
			}			
			FontCharacter chr = { { glyph->s0, glyph->t1, glyph->s1, glyph->t0 }, glyph->width, glyph->height, glyph->advance_x, glyph->advance_y, glyph->offset_x, glyph->offset_y, kerning };
			result.push_back(chr);
		}
		return result;
	}

	Vector2f Font::SizeOfText(const blt::string& text) const
	{
		std::vector<Font::FontCharacter> characters = GetCharacters(text);
		Vector2f size = std::accumulate(characters.begin(), characters.end(), Vector2f(), [](const Vector2f& current, const Font::FontCharacter& chr) {
			Vector2f result;
			result.y = current.y;
			if (chr.Height > current.y)
			{
				result.y = chr.Height;
			}
			result.x = current.x + chr.OffsetX + chr.AdvanceX + chr.Kerning;
			return result;
		});
		return size;
	}

}