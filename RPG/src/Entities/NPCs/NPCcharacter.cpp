#include "NPCcharacter.h"
#include "..\PlayerWizard.h"
#include "..\..\GameManager.h"

namespace RPG
{

	NPCcharacter::NPCcharacter() : Character()
	{
	
	}

	NPCcharacter::NPCcharacter(const MapTile* tile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, std::unique_ptr<NPCbehaviour>&& behaviour, bool isAttackable, float maxHealth, bool showHealthbar, id_t layerId)
		: Character(tile, size, speed, orientation, textureSet, isAttackable, maxHealth, showHealthbar, layerId),
		m_Behaviour(std::move(behaviour))
	{
		
	}


	void NPCcharacter::SetBehaviour(std::unique_ptr<NPCbehaviour>&& behaviour)
	{
		m_Behaviour = std::move(behaviour);
	}

	void NPCcharacter::NPCUpdate(const PlayerWizard* player)
	{
		if (m_Behaviour.get() != nullptr)
		{
			m_Behaviour->Update(this, player, &GameManager::TileManager.CurrentTileMap());
		}
	}

}