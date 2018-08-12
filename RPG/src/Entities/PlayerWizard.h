#pragma once
#include "Character.h"

namespace RPG
{

	class PlayerWizard : public Character
	{
	public:
		PlayerWizard();
		PlayerWizard(const MapTile* currentTile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, id_t layerId = 0);

		void Update() override;

	};

}