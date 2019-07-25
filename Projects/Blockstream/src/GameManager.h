#pragma once
#include "MapTileManager.h"
#include "Events.h"
#include "GameServer.h"

namespace Blockstream
{

	class BlockstreamClient;

	class GameManager
	{
	private:
		static BlockstreamClient* s_Client;

		static MapTile* s_StartTile;
		static MapTile* s_EndTile;
		static MapTile* s_LastMove;
		static MapTile* s_LastOpponentMove;

		static MapTileManager s_TileManager;
		static bool s_IsPaused;
		static bool s_IsTurn;

		static GameServer s_Server;

	public:
		GameManager() = delete;

		static void Init(BlockstreamClient* client);
		static void Terminate();
		static MapTileManager& Map();
		static GameServer& Server();

		static MapTile* StartTile();
		static MapTile* EndTile();
		static void SetStartTile(MapTile* tile);
		static void SetEndTile(MapTile* tile);
		static void SetPause(bool pause);
		static void SetTurn(bool isTurn);

		static bool CanDecrementTile(MapTile* tile, blt::string* outReason);
		static bool CanIncrementTile(MapTile* tile, blt::string* outReason);
		static void DecrementTile(MapTile* tile);
		static void IncrementTile(MapTile* tile);

	};

}