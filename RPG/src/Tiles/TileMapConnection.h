#pragma once
#include "..\Entities\CharacterTextureSet.h"
#include "TileMapLevel.h"

namespace RPG
{

	struct TileMapConnection
	{
	public:
		Vector2i TriggerPosition;
		Vector2i DestinationPosition;
		CharacterOrientation RequiredOrientation;
		TileMapLevel* DestinationLevel;

	};

}