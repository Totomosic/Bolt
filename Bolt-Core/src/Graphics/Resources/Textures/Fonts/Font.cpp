#include "bltpch.h"
#include "Font.h"

namespace Bolt
{

	Font::Font(const Filepath& fontFile, float fontSize, int textureWidth, int textureHeight) : Texture2D(textureWidth, textureHeight, TextureFormat::R, { WrapMode::ClampToEdge, MagFilter::Linear, MinFilter::Linear, Mipmaps::Disabled }),
		m_FontSize(fontSize)
	{
		BLT_ASSERT(Filesystem::FileExists(fontFile), "Unable to find font file " + fontFile.Path());
		m_TextureAtlas = std::unique_ptr<texture_atlas_t, std::function<void(texture_atlas_t*)>>(texture_atlas_new(m_Width, m_Height, 1), [](texture_atlas_t* ptr) { texture_atlas_delete(ptr); });
		m_TextureFont = std::unique_ptr<texture_font_t, std::function<void(texture_font_t*)>>(texture_font_new_from_file(m_TextureAtlas.get(), m_FontSize, fontFile.Path().c_str()), [](texture_font_t* ptr) { texture_font_delete(ptr); });
		texture_font_load_glyphs(m_TextureFont.get(), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=+';:,.<>?/`~|\\!@#$%^&*()_\"[]{}");
		m_TextureAtlas->id = m_Id;
		GL_CALL(glTexSubImage2D((GLenum)Target(), 0, 0, 0, Width(), Height(), GL_RED, GL_UNSIGNED_BYTE, m_TextureAtlas->data));
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
			texture_glyph_t* glyph = texture_font_get_glyph(m_TextureFont.get(), &c);
			float kerning = 0;
			if (i != 0)
			{
				char prevC = str[i - 1];
				kerning = texture_glyph_get_kerning(glyph, &prevC);
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
			result.x = current.x + chr.AdvanceX + chr.Kerning;
			return result;
		});
		return size;
	}

}