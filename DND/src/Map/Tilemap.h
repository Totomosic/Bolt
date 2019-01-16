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
		};

	private:
		ObjectFactory m_Factory;
		std::vector<LayerInfo> m_Layers;
		int m_Width;
		int m_Height;
		int m_TileWidth;
		int m_TileHeight;

	public:
		Tilemap(Layer& layer, int width, int height, int tileWidth, int tileHeight);

		int Width() const;
		int Height() const;
		int TileWidth() const;
		int TileHeight() const;
		const std::vector<LayerInfo>& Layers() const;
		const TilemapLayer& GetLayer(int index) const;
		TilemapLayer& GetLayer(int index);
		GameObject* GetLayerObject(int index) const;

		Vector3f WorldPositionOfTile(int x, int y) const;
		Tile TileFromWorldPosition(float x, float y) const;

		TilemapLayer& AddLayer(float resolution);
		void Clear();

	};

}