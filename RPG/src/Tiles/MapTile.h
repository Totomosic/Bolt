#pragma once
#include "..\Entities\Sprite.h"

namespace RPG
{

	class Character;

	enum class TileType
	{
		Grass,
		Sand,
		Water,
		StonePath
	};

	class MapTile : public Sprite
	{
	private:
		// Tile Position in Level
		Vector2i m_Position;
		TileType m_Type;
		mutable bool m_IsEnabled;
		mutable Character* m_AboveObject;

	public:
		MapTile();
		MapTile(Vector2i position, Vector2f size, TileType type, id_t layerId = 0);

		const Vector2i& TilePosition() const;
		TileType Type() const;
		bool IsEnabled() const;
		Character* ObjectOnTile() const;

		void Enable() const;
		void Disable() const;
		void SetType(TileType type);

		void SetObjectOnTile(Character* character) const;

	public:
		static const Texture2D* GetTileTexture(TileType type);

	};

}