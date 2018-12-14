#include "Tilemap.h"

namespace DND
{

	Tilemap::Tilemap(const ObjectFactory& factory, const ResourcePtr<const Texture2D>& mapTexture, float tileSize, TilemapSetupInfo info)
		: m_Width(mapTexture->Width()), m_Height(mapTexture->Height()), m_TileSize(tileSize), m_TileArray(new Tilemap::TileInfo[m_Width * m_Height]), m_Object(nullptr), m_TexturePtr(nullptr)
	{
		ResourcePtr<const Model> model = CreateMapModel(mapTexture.Get());
		Mesh mapMesh;
		mapMesh.Models.push_back({ std::move(model), Matrix4f::Identity(), { 0 } });
		mapMesh.Materials[0].Shader = ResourceManager::Get<const Shader>(info.ShaderId);
		mapMesh.Materials[0].Textures.Textures.push_back(mapTexture);
		mapMesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<const Texture2D>(info.GrassTextureId));
		mapMesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<const Texture2D>(info.SandTextureId));
		mapMesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<const Texture2D>(info.WaterTextureId));
		mapMesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<const Texture2D>(info.PathTextureId));
		m_Object = factory.Instantiate(mapMesh);
		m_TexturePtr = mapTexture.Get();
	}

	int Tilemap::Width() const
	{
		return m_Width;
	}

	int Tilemap::Height() const
	{
		return m_Height;
	}

	float Tilemap::Tilesize() const
	{
		return m_TileSize;
	}

	const Tilemap::TileInfo* Tilemap::Tiles() const
	{
		return m_TileArray.get();
	}

	GameObject* Tilemap::Object() const
	{
		return m_Object;
	}

	Vector3f Tilemap::WorldPositionOfTile(const Tilemap::TileInfo& tile) const
	{
		return m_Object->transform().Position() + Vector3f((tile.x - Width() / 2.0f) * Tilesize(), (tile.y - Height() / 2.0f) * Tilesize(), 0);
	}

	Vector3f Tilemap::WorldPositionOfTile(int x, int y) const
	{
		return WorldPositionOfTile(GetTile(x, y));
	}

	bool Tilemap::IsTile(float worldX, float worldY) const
	{
		Vector3f pos = m_Object->transform().Position();
		float x = worldX - pos.x + Width() / 2.0f * Tilesize() + Tilesize() / 2;
		float y = worldY - pos.y + Height() / 2.0f * Tilesize() + Tilesize() / 2;
		int xind = std::floor(x / Tilesize());
		int yind = std::floor(y / Tilesize());
		return (xind >= 0 && yind >= 0) && (xind < Width() && yind < Height());
	}

	const Tilemap::TileInfo& Tilemap::GetTileFromWorldPosition(float x, float y) const
	{
		Vector3f pos = m_Object->transform().Position();
		x = x - pos.x + Width() / 2.0f * Tilesize() + Tilesize() / 2;
		y = y - pos.y + Height() / 2.0f * Tilesize() + Tilesize() / 2;
		int xind = std::floor(x / Tilesize());
		int yind = std::floor(y / Tilesize());
		return GetTile(xind, yind);
	}

	const Tilemap::TileInfo& Tilemap::GetTile(int x, int y) const
	{
		return m_TileArray[x + y * Width()];
	}

	ResourcePtr<const Model> Tilemap::CreateMapModel(const Texture2D* texture)
	{
		texture->LoadPixels();
		for (int y = 0; y < Height(); y++)
		{
			for (int x = 0; x < Width(); x++)
			{
				TileInfo& tile = m_TileArray[x + y * Width()];
				tile.Type = INVALID_TILE_TYPE;
				tile.x = x;
				tile.y = y;
			}
		}
		Model* model = new Model(RectangleFactory(Width() * Tilesize(), Height() * Tilesize()));
		BufferLayout layout;
		layout.AddAttribute<float>(4, 2);
		layout.AddAttribute<float>(5, 2);
		model->Data().Vertices->CreateVertexBuffer(4 * layout.Size(), layout);
		{
			VertexIterator it = model->Data().Vertices->Begin();
			for (int i = 0; i < 4; i++)
			{
				it[4] = Vector2f(Width(), Height());
				it[5] = Vector2f(Width() * Tilesize(), Height() * Tilesize());
				it++;
			}
		}
		return ResourcePtr<const Model>(model, true);
	}

	void Tilemap::RecolourPixel(int x, int y, const Color& color, const Texture2D* texture) const
	{
		texture->LoadPixels();
		texture->Pixels[x + y * texture->Width()] = color;
		texture->UpdatePixels();
	}

}