#pragma once
#include "TileMapLevel.h"
#include "..\Entities\Character.h"

namespace RPG
{

	class TileCamera
	{
	private:
		Character* m_FollowedObject;
		Vector2i m_TilePosition;
		Vector2i m_ViewSize; // In Tiles

		Camera* m_CameraObject;

		const MapTile* m_PreviousCharacterTile;
		const TileMapLevel* m_PreviousTileMap;

	public:
		TileCamera(Character* character, Camera* camera, const Vector2i& viewSize);

		Character* CharacterObject() const;
		Camera* CameraObject() const;

		void Update(TileMapLevel* currentLevel);
		void RecreateView(TileMapLevel* currentLevel);

	};

}