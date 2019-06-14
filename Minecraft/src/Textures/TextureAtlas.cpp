#include "mcpch.h"
#include "TextureAtlas.h"

namespace Minecraft
{

	TextureAtlas::TextureAtlas() : TextureAtlas(nullptr, 1, 1)
	{
	
	}

	TextureAtlas::TextureAtlas(const ResourcePtr<Texture2D>& texture, int individualWidth, int individualHeight)
		: m_Texture(texture), m_IndividualWidth(individualWidth), m_IndividualHeight(individualHeight)
	{
	
	}

	int TextureAtlas::GetIndividualWidth() const
	{
		return m_IndividualWidth;
	}

	int TextureAtlas::GetIndividualHeight() const
	{
		return m_IndividualHeight;
	}

	const ResourcePtr<Texture2D>& TextureAtlas::GetTexture() const
	{
		return m_Texture;
	}

	int TextureAtlas::GetImagesPerRow() const
	{
		return GetTexture()->Width() / m_IndividualWidth;
	}

	int TextureAtlas::GetImagesPerCol() const
	{
		return GetTexture()->Height() / m_IndividualHeight;
	}

	int TextureAtlas::GetTotalImages() const
	{
		return GetImagesPerRow() * GetImagesPerCol();
	}

	TextureCoordinate TextureAtlas::GetImage(int index) const
	{
		Vector2i indices = GetXYIndicesFromIndex(index);
		return { 
			{ ((indices.x) * GetIndividualWidth() + 1) / (float)GetTexture()->Width(), ((GetImagesPerCol() - indices.y - 1) * GetIndividualHeight() + 1) / (float)GetTexture()->Height() }, 
			{ ((indices.x + 1) * GetIndividualWidth() - 1) / (float)GetTexture()->Width(), ((GetImagesPerCol() - indices.y) * GetIndividualHeight() - 1) / (float)GetTexture()->Height() }
		};
	}

	Vector2i TextureAtlas::GetXYIndicesFromIndex(int index) const
	{
		return { index % GetImagesPerRow(), index / GetImagesPerRow() };
	}

}