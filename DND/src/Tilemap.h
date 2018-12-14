#pragma once
#include "BoltInclude.h"

namespace DND
{

	constexpr int INVALID_TILE_TYPE = -1;

	struct TilemapSetupInfo
	{
	public:
		id_t ShaderId;
		id_t GrassTextureId;
		id_t SandTextureId;
		id_t WaterTextureId;
		id_t PathTextureId;
	};

	class Tilemap
	{
	public:
		struct TileInfo
		{
		public:
			int Type;
			int x;
			int y;
		};

	private:
		int m_Width;
		int m_Height;
		float m_TileSize;
		std::unique_ptr<TileInfo[]> m_TileArray;
		GameObject* m_Object;
		const Texture2D* m_TexturePtr;

	public:
		Tilemap(const ObjectFactory& factory, const ResourcePtr<const Texture2D>& map, float tileSize, TilemapSetupInfo info);

		int Width() const;
		int Height() const;
		float Tilesize() const;
		const TileInfo* Tiles() const;
		GameObject* Object() const;

		Vector3f WorldPositionOfTile(const TileInfo& tile) const;
		Vector3f WorldPositionOfTile(int x, int y) const;
		bool IsTile(float worldX, float worldY) const;
		const TileInfo& GetTileFromWorldPosition(float x, float y) const;

		// Gets tile at given x,y from Bottom-Left
		const TileInfo& GetTile(int x, int y) const;

	private:
		ResourcePtr<const Model> CreateMapModel(const Texture2D* texture);
		void RecolourPixel(int x, int y, const Color& color, const Texture2D* texture) const;

	};

}