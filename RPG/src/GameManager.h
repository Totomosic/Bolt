#pragma once
#include "Tiles\TileMapManager.h"
#include "Entities\PlayerWizard.h"
#include "Tiles\TileCamera.h"

namespace RPG
{

	class GameManager
	{
	public:
		static TileMapManager TileManager;
		static std::unique_ptr<PlayerWizard> Player;
		static std::unique_ptr<TileCamera> Camera;

		static MapTile* SelectedTile;
		static Character* SelectedCharacter;

	};

}