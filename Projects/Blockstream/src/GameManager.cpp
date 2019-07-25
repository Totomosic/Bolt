#include "GameManager.h"
#include "App.h"

namespace Blockstream
{

	BlockstreamClient* GameManager::s_Client = nullptr;

	MapTile* GameManager::s_StartTile = nullptr;
	MapTile* GameManager::s_EndTile = nullptr;
	MapTile* GameManager::s_LastMove = nullptr;
	MapTile* GameManager::s_LastOpponentMove = nullptr;

	MapTileManager GameManager::s_TileManager = MapTileManager();
	bool GameManager::s_IsPaused = true;
	bool GameManager::s_IsTurn = false;

	GameServer GameManager::s_Server = GameServer();

	void GameManager::Init(BlockstreamClient* client)
	{
		s_Client = client;
		Map().Init();
	}

	void GameManager::Terminate()
	{
		s_Server.Terminate();
	}

	MapTileManager& GameManager::Map()
	{
		return s_TileManager;
	}

	GameServer& GameManager::Server()
	{
		return s_Server;
	}

	MapTile* GameManager::StartTile()
	{
		return s_StartTile;
	}

	MapTile* GameManager::EndTile()
	{
		return s_EndTile;
	}

	void GameManager::SetStartTile(MapTile* tile)
	{
		s_StartTile = tile;
		Map().SetStartTile(tile);
	}

	void GameManager::SetEndTile(MapTile* tile)
	{
		s_EndTile = tile;
		tile->MakeEndTile();
	}

	void GameManager::SetPause(bool pause)
	{
		s_IsPaused = pause;
	}

	void GameManager::SetTurn(bool isTurn)
	{
		s_IsTurn = isTurn;
	}

	bool GameManager::CanDecrementTile(MapTile* tile, blt::string* outReason)
	{
		if (s_IsPaused)
		{
			*outReason = "Game is Paused";
			return false;
		}
		if (!s_IsTurn)
		{
			*outReason = "Opponent's turn";
			return false;
		}
		if (tile->GetLevel() <= -4)
		{
			*outReason = "Tile is already too low";
			return false;
		}
		if (tile == s_LastMove)
		{
			*outReason = "Tile was moved by you last turn";
			return false;
		}
		if (tile == s_LastOpponentMove)
		{
			*outReason = "Tile was move by opponent last turn";
			return false;
		}
		s_IsTurn = false;
		s_LastMove = tile;
		tile->DecrementLevel();
		Server().MoveBlock(tile, true);
		return true;
	}

	bool GameManager::CanIncrementTile(MapTile* tile, blt::string* outReason)
	{
		if (s_IsPaused)
		{
			*outReason = "Game is Paused";
			return false;
		}
		if (!s_IsTurn)
		{
			*outReason = "Opponent's turn";
			return false;
		}
		if (tile->GetLevel() >= 4)
		{
			*outReason = "Tile is already too high";
			return false;
		}
		if (tile == s_LastMove)
		{
			*outReason = "Tile was moved by you last turn";
			return false;
		}
		if (tile == s_LastOpponentMove)
		{
			*outReason = "Tile was move by opponent last turn";
			return false;
		}
		s_IsTurn = false;
		s_LastMove = tile;
		tile->IncrementLevel();
		Server().MoveBlock(tile, false);
		return true;
	}

	void GameManager::DecrementTile(MapTile* tile)
	{
		s_IsTurn = true;
		s_LastOpponentMove = tile;
		tile->DecrementLevel();
	}

	void GameManager::IncrementTile(MapTile* tile)
	{
		s_IsTurn = true;
		s_LastOpponentMove = tile;
		tile->IncrementLevel();
	}

}