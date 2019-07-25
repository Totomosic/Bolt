#pragma once
#include "TilemapLayer.h"

namespace DND
{

	class Tilemap
	{
	public:
		struct LayerInfo
		{
			GameObject* Object;
			TilemapLayer TLayer;
			float Resolution;
		};

	private:
		std::vector<LayerInfo> m_Layers;
		int m_Width;
		int m_Height;
		int m_TileWidth;
		int m_TileHeight;

	public:
		Tilemap();
		Tilemap(int width, int height);

		int Width() const;
		int Height() const;
		int TileWidth() const;
		int TileHeight() const;
		const std::vector<LayerInfo>& Layers() const;
		const TilemapLayer& GetLayer(int index) const;
		TilemapLayer& GetLayer(int index);
		GameObject* GetLayerObject(int index) const;

		Vector3f WorldPositionOfTile(int x, int y) const;
		Vector3f WorldPositionOfTile(const Tile& tile) const;
		Tile TileFromWorldPosition(float x, float y) const;
		Tile TileFromWorldPosition(const Vector2f& worldPos) const;

		TilemapLayer& AddLayer(float resolution);
		void Clear();

		void Show();
		void Hide();
		void Load(const ObjectFactory& factory, const std::unordered_map<id_t, Image>& tiles);
		void LoadAsync(const ObjectFactory& factory, const std::unordered_map<id_t, Image>& tiles, std::function<void()> callback);
		void Unload();

		friend class TilemapManager;

		Image CreateImage(const LayerInfo& layer, const std::unordered_map<id_t, Image>& tiles);
		Task<std::vector<Image>> CreateImagesAsync(const std::vector<LayerInfo>& layers, const std::unordered_map<id_t, Image>& tiles);

	private:
		void SetTileSize(const Vector2i& size);

	};

}