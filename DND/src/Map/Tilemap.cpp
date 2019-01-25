#include "bltpch.h"
#include "Tilemap.h"

namespace DND
{

	Tilemap::Tilemap()
	{

	}

	Tilemap::Tilemap(int width, int height)
		: m_Layers(), m_Width(width), m_Height(height), m_TileWidth(0), m_TileHeight(0)
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
		float wx = ((float)x - (float)Width() / 2.0f) * m_TileWidth + m_TileWidth / 2.0f;
		float wy = ((float)y - (float)Height() / 2.0f) * m_TileHeight + m_TileHeight / 2.0f;
		return Vector3f(wx, wy, 0);
	}

	Tile Tilemap::TileFromWorldPosition(float x, float y) const
	{
		int tileX = (x + Width() * m_TileWidth / 2.0f) / m_TileWidth;
		int tileY = (y + Height() * m_TileHeight / 2.0f) / m_TileHeight;
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
		Tilemap::LayerInfo i = { nullptr, TilemapLayer(Width(), Height()), resolution };
		m_Layers.push_back(std::move(i));
		Tilemap::LayerInfo& info = m_Layers.at(index);
		return info.TLayer;
	}

	void Tilemap::Clear()
	{
		m_Layers.clear();
	}

	void Tilemap::Show()
	{
		for (LayerInfo& info : m_Layers)
		{
			if (info.Object != nullptr)
			{
				info.Object->Components().GetComponent<MeshRenderer>().Enable();
			}
		}
	}
	
	void Tilemap::Hide()
	{
		for (LayerInfo& info : m_Layers)
		{
			if (info.Object != nullptr)
			{
				info.Object->Components().GetComponent<MeshRenderer>().Disable();
			}
		}
	}

	void Tilemap::Load(const ObjectFactory& factory, const std::unordered_map<id_t, Image>& tiles)
	{
		for (LayerInfo& layer : m_Layers)
		{
			if (layer.Object == nullptr)
			{
				Image image = CreateImage(layer, tiles);
				TextureCreateOptions options;
				Texture2D* texture = new Texture2D(image, options);
				layer.Object = factory.Image(Width() * m_TileWidth, Height() * m_TileHeight, ResourcePtr<const Texture2D>((const Texture2D*)texture, true), Transform({ 0, 0, -25 }));
			}
		}
	}

	void Tilemap::Unload()
	{
		for (LayerInfo& layer : m_Layers)
		{
			if (layer.Object != nullptr)
			{
				Destroy(layer.Object);
				layer.Object = nullptr;
			}
		}
	}

	Image Tilemap::CreateImage(const Tilemap::LayerInfo& layer, const std::unordered_map<id_t, Image>& tiles)
	{
		int xPixelsPerTile = m_TileWidth * layer.Resolution;
		int yPixelsPerTile = m_TileHeight * layer.Resolution;
		Image image;
		image.Width = Width() * xPixelsPerTile;
		image.Height = Height() * yPixelsPerTile;
		image.Components = 4;
		image.Pixels = new byte[image.Width * image.Height * image.Components];
		std::unordered_map<id_t, Image> resizedImages;
		for (int i = 0; i < Width(); i++)
		{
			for (int j = 0; j < Height(); j++)
			{
				id_t tileId = layer.TLayer.GetTile(i, j);
				auto it = resizedImages.find(tileId);
				if (it == resizedImages.end())
				{
					resizedImages[tileId] = tiles.at(tileId).Resize(xPixelsPerTile, yPixelsPerTile, ResizeFilter::Nearest);
					it = resizedImages.find(tileId);
				}
				const Image& tileImage = it->second;
				for (int h = 0; h < tileImage.Height; h++)
				{
					memcpy(image.Pixels + (i + (h + j * yPixelsPerTile) * Width()) * xPixelsPerTile * 4, tileImage.Pixels + (h * xPixelsPerTile) * 4, xPixelsPerTile * 4);
				}
			}
		}
		return image;
	}

	void Tilemap::SetTileSize(const Vector2i& size)
	{
		m_TileWidth = size.x;
		m_TileHeight = size.y;
	}

}