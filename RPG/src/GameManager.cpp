#include "GameManager.h"

namespace RPG
{

	TileMapManager GameManager::TileManager = TileMapManager();
	std::unique_ptr<PlayerWizard> GameManager::Player = std::unique_ptr<PlayerWizard>();
	std::unique_ptr<TileCamera> GameManager::Camera = std::unique_ptr<TileCamera>();

	MapTile* GameManager::SelectedTile = nullptr;
	Character* GameManager::SelectedCharacter = nullptr;

}