#include "Tilemap.h"

namespace DND
{

	Tilemap::Tilemap(Layer& layer, int width, int height, int tileWidth, int tileHeight)
		: m_Factory(layer), m_Layers(), m_Width(width), m_Height(height), m_TileWidth(tileWidth), m_TileHeight(tileHeight)
	{
		
	}

	int Tilemap::Width() const
	{
		return m_Width;
	}

	int Tilemap::Height() const
	{
		return m_Height;
	}

	int Tilemap::TileWidth() const
	{
		return m_TileWidth;
	}

	int Tilemap::TileHeight() const
	{
		return m_TileHeight;
	}

	const std::vector<Tilemap::LayerInfo>& Tilemap::Layers() const
	{
		return m_Layers;
	}

	const TilemapLayer& Tilemap::GetLayer(int index) const
	{
		return m_Layers.at(index).TLayer;
	}

	TilemapLayer& Tilemap::GetLayer(int index)
	{
		return m_Layers.at(index).TLayer;
	}

	GameObject* Tilemap::GetLayerObject(int index) const
	{
		return m_Layers.at(index).Object;
	}

	TilemapLayer& Tilemap::AddLayer(float resolution)
	{
		size_t index = m_Layers.size();
		Tilemap::LayerInfo i = { nullptr, TilemapLayer(Width(), Height(), TileWidth(), TileHeight(), resolution) };
		m_Layers.push_back(std::move(i));
		Tilemap::LayerInfo& info =  m_Layers.at(index);
		info.Object = m_Factory.Image(Width() * TileWidth(), Height() * TileHeight(), &info.TLayer.GetTexture(), Transform());
		return info.TLayer;
	}

}