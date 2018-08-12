#include "MapTile.h"
#include "..\Entities\Character.h"

namespace RPG
{

	MapTile::MapTile() : Sprite()
	{
	
	}

	MapTile::MapTile(Vector2i position, Vector2f size, TileType type, id_t layerId) : Sprite(position * size + size / 2.0f, size, GetTileTexture(type), layerId),
		m_Position(position), m_Type(type), m_IsEnabled(true), m_AboveObject(nullptr)
	{
	
	}

	const Vector2i& MapTile::TilePosition() const
	{
		return m_Position;
	}

	TileType MapTile::Type() const
	{
		return m_Type;
	}

	bool MapTile::IsEnabled() const
	{
		return m_IsEnabled;
	}

	Character* MapTile::ObjectOnTile() const
	{
		return m_AboveObject;
	}

	void MapTile::Enable() const
	{
		m_IsEnabled = true;
	}

	void MapTile::Disable() const
	{
		m_IsEnabled = false;
	}
	
	void MapTile::SetType(TileType type)
	{
		m_Type = type;
		SetTexture(GetTileTexture(m_Type));
	}

	void MapTile::SetObjectOnTile(Character* character) const
	{
		m_AboveObject = character;
		if (character != nullptr)
		{
			Disable();
		}
		else
		{
			Enable();
		}
	}

	const Texture2D* MapTile::GetTileTexture(TileType type)
	{
		switch (type)
		{
		case TileType::Grass:
			return ResourceManager::Get<Texture2D>("Grass_Tile_Texture");
		case TileType::Sand:
			return ResourceManager::Get<Texture2D>("Sand_Tile_Texture");
		case TileType::Water:
			return ResourceManager::Get<Texture2D>("Water_Tile_Texture");
		case TileType::StonePath:
			return ResourceManager::Get<Texture2D>("Stone_Path_Tile_Texture");
		default:
			return nullptr;
		}
	}

}