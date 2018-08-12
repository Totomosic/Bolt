#pragma once
#include "..\Character.h"
#include "Behaviours\NPCbehaviour.h"

namespace RPG
{

	class PlayerWizard;

	class NPCcharacter : public Character
	{
	protected:
		std::unique_ptr<NPCbehaviour> m_Behaviour;

	public:
		NPCcharacter();
		NPCcharacter(const MapTile* tile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, std::unique_ptr<NPCbehaviour>&& behaviour = std::unique_ptr<NPCbehaviour>(), 
			bool isAttackable = false, float maxHealth = 100, bool showHealthbar = true, id_t layerId = 0);

		void SetBehaviour(std::unique_ptr<NPCbehaviour>&& behaviour);
		void NPCUpdate(const PlayerWizard* player);

	};

}