#include "PlayerWizard.h"
#include "..\GameManager.h"

namespace RPG
{

	PlayerWizard::PlayerWizard() : Character()
	{
	
	}

	PlayerWizard::PlayerWizard(const MapTile* currentTile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, id_t layerId) : Character(currentTile, size, speed, orientation, textureSet, true, 100, true, layerId)
	{
	
	}

	void PlayerWizard::Update()
	{
		if (m_TargetTile == nullptr)
		{
			if (Input::KeyDown(Keycode::W))
			{
				SetOrientation(CharacterOrientation::Up);
				SetTargetTile(&GameManager::TileManager.CurrentTileMap().GetTile(m_CurrentTile->TilePosition().x, m_CurrentTile->TilePosition().y + 1));
			}
			else if (Input::KeyDown(Keycode::S))
			{
				SetOrientation(CharacterOrientation::Down);
				SetTargetTile(&GameManager::TileManager.CurrentTileMap().GetTile(m_CurrentTile->TilePosition().x, m_CurrentTile->TilePosition().y - 1));
			}
			else if (Input::KeyDown(Keycode::D))
			{
				SetOrientation(CharacterOrientation::Right);
				SetTargetTile(&GameManager::TileManager.CurrentTileMap().GetTile(m_CurrentTile->TilePosition().x + 1, m_CurrentTile->TilePosition().y));
			}
			else if (Input::KeyDown(Keycode::A))
			{
				SetOrientation(CharacterOrientation::Left);
				SetTargetTile(&GameManager::TileManager.CurrentTileMap().GetTile(m_CurrentTile->TilePosition().x - 1, m_CurrentTile->TilePosition().y));
			}
		}
		Character::Update();
	}

}