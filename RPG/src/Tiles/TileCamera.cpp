#include "TileCamera.h"

namespace RPG
{

	TileCamera::TileCamera(Character* character, Camera* camera, const Vector2i& viewSize)
		: m_FollowedObject(character), m_TilePosition(), m_ViewSize(viewSize), m_CameraObject(camera), m_PreviousCharacterTile(nullptr), m_PreviousTileMap(nullptr)
	{
	
	}

	Character* TileCamera::CharacterObject() const
	{
		return m_FollowedObject;
	}

	Camera* TileCamera::CameraObject() const
	{
		return m_CameraObject;
	}

	void TileCamera::Update(TileMapLevel* currentLevel)
	{
		int tilemapWidth = currentLevel->Width();
		int tilemapHeight = currentLevel->Height();
		int viewWidth = m_ViewSize.x;
		int viewHeight = m_ViewSize.y;
		float tileSize = 40;
		float x = m_FollowedObject->Position().x - tileSize / 2.0f;
		float y = m_FollowedObject->Position().y - tileSize / 2.0f;
		if (x - viewWidth / 2 * tileSize < 0)
		{
			x = viewWidth / 2 * tileSize;
		}
		if (x + viewWidth / 2 * tileSize > tilemapWidth * tileSize)
		{
			x = tilemapWidth * tileSize - viewWidth / 2 * tileSize;
		}
		if (y - viewHeight / 2 * tileSize < 0)
		{
			y = viewHeight / 2 * tileSize;
		}
		if (y + viewHeight / 2 * tileSize > tilemapHeight * tileSize)
		{
			y = tilemapHeight * tileSize - viewHeight / 2 * tileSize;
		}

		Vector3f prevPosition = m_CameraObject->transform().Position();
		m_CameraObject->transform().SetLocalPosition(x, y, 0);
		int tileX = x / tileSize;
		int tileY = y / tileSize;
		m_TilePosition = Vector2i(tileX, tileY);

		if ((m_FollowedObject->CurrentTile() != m_PreviousCharacterTile && (x != prevPosition.x || y != prevPosition.y)) || (currentLevel != m_PreviousTileMap))
		{
			TileMapLevel& tilemap = *currentLevel;
			if (x < prevPosition.x)
			{
				tilemap.Destroy(tileX + m_ViewSize.x / 2 + 1, tileY - m_ViewSize.y / 2 - 1, 1, m_ViewSize.y + 2);
			}
			else if (x > prevPosition.x)
			{
				tilemap.Destroy(tileX - m_ViewSize.x / 2 - 2, tileY - m_ViewSize.y / 2 - 1, 1, m_ViewSize.y + 2);
			}
			else if (y < prevPosition.y)
			{
				tilemap.Destroy(tileX - m_ViewSize.x / 2 - 1, tileY + m_ViewSize.y / 2 + 1, m_ViewSize.x + 2, 1);
			}
			else if (y > prevPosition.y)
			{
				tilemap.Destroy(tileX - m_ViewSize.x / 2 - 1, tileY - m_ViewSize.y / 2 - 2, m_ViewSize.x + 2, 1);
			}
			tilemap.Create(tileX - m_ViewSize.x / 2 - 1, tileY - m_ViewSize.y / 2 - 1, m_ViewSize.x + 2, m_ViewSize.y + 2);
		}
		
		m_PreviousCharacterTile = m_FollowedObject->CurrentTile();
		m_PreviousTileMap = currentLevel;
	}

	void TileCamera::RecreateView(TileMapLevel* currentLevel)
	{
		currentLevel->Destroy();
		currentLevel->Create(m_TilePosition.x - m_ViewSize.x / 2 - 1, m_TilePosition.y - m_ViewSize.y / 2 - 1, m_ViewSize.x + 2, m_ViewSize.y + 2);
	}

}