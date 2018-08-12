#pragma once
#include "..\..\PlayerWizard.h"

namespace RPG
{

	class NPCcharacter;
	class TileMapLevel;

	class NPCbehaviour
	{
	protected:

	public:
		NPCbehaviour();

		virtual void Update(NPCcharacter* character, const PlayerWizard* player, const TileMapLevel* level) = 0;

	};

}