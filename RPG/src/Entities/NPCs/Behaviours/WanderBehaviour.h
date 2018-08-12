#pragma once
#include "NPCbehaviour.h"

namespace RPG
{

	class WanderBehaviour : public NPCbehaviour
	{
	private:
		float m_MinMoveTime;
		float m_MaxMoveTime;
		float m_ChanceToContinue;

		float m_MoveTime;
		float m_CurrentTime;

	public:
		WanderBehaviour(float minMoveTime = 0, float maxMoveTime = 3, float continueChance = 0.25f);

		void Update(NPCcharacter* character, const PlayerWizard* player, const TileMapLevel* level) override;

	};

}