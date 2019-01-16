#include "bltpch.h"
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

	Vector3f Tilemap::WorldPositionOfTile(int x, int y) const
	{
		float wx = ((float)x - (float)Width() / 2.0f) * TileWidth() + TileWidth() / 2.0f;
		float wy = ((float)y - (float)Height() / 2.0f) * TileHeight() + TileHeight() / 2.0f;
		return Vector3f(wx, wy, 0);
	}

	Tile Tilemap::TileFromWorldPosition(float x, float y) const
	{
		int tileX = (x + Width() * TileWidth() / 2.0f) / TileWidth();
		int tileY = (y + Height() * TileHeight() / 2.0f) / TileHeight();
		if (tileX < 0 || tileX >= Width())
		{
			tileX = -1;
		}
		if (tileY < 0 || tileY >= Height())
		{
			tileY = -1;
		}
		return Tile(tileX, tileY);
	}

	TilemapLayer& Tilemap::AddLayer(float resolution)
	{
		size_t index = m_Layers.size();
		Tilemap::LayerInfo i = { nullptr, TilemapLayer(Width(), Height(), TileWidth(), TileHeight(), resolution) };
		m_Layers.push_back(std::move(i));
		Tilemap::LayerInfo& info =  m_Layers.at(index);
		info.Object = m_Factory.Image(Width() * TileWidth(), Height() * TileHeight(), &info.TLayer.GetTexture(), Transform({ 0, 0, -25 }));
		return info.TLayer;
	}

	void Tilemap::Clear()
	{
		m_Layers.clear();
	}

}