#include "WanderBehaviour.h"
#include "..\NPCcharacter.h"
#include "..\..\PlayerWizard.h"
#include "..\..\..\Tiles\TileMapLevel.h"

namespace RPG
{

	WanderBehaviour::WanderBehaviour(float minMoveTime, float maxMoveTime, float continueChance) : NPCbehaviour(),
		m_MinMoveTime(minMoveTime), m_MaxMoveTime(maxMoveTime), m_ChanceToContinue(continueChance), m_MoveTime(Random::NextFloat(m_MinMoveTime, m_MaxMoveTime)), m_CurrentTime(0.0f)
	{
	
	}

	void WanderBehaviour::Update(NPCcharacter* character, const PlayerWizard* player, const TileMapLevel* level)
	{
		if (character->TargetTile() == nullptr)
		{
			m_CurrentTime += Time::DeltaTime();
			if (m_CurrentTime >= m_MoveTime)
			{
				int direction = Random::NextInt(0, 3);
				const MapTile* target = nullptr;
				int currentX = character->CurrentTile()->TilePosition().x;
				int currentY = character->CurrentTile()->TilePosition().y;
				switch (direction)
				{
				case 0:
					target = &level->GetTile(currentX - 1, currentY);
					break;
				case 1:
					target = &level->GetTile(currentX + 1, currentY);
					break;
				case 2:
					target = &level->GetTile(currentX, currentY - 1);
					break;
				case 3:
					target = &level->GetTile(currentX, currentY + 1);
					break;
				}
				character->SetTargetTile(target);
				m_CurrentTime = 0;
				m_MoveTime = Random::NextFloat(m_MinMoveTime, m_MaxMoveTime);
				if (Random::NextFloat(0.0f, 1.0f) <= m_ChanceToContinue)
				{
					m_MoveTime = 0;
				}
			}
		}
	}

}