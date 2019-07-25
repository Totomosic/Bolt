#pragma once
#include "BoltInclude.h"

namespace Blockstream
{

	const id_t TILEMAP_UPDATE_EVENT = EventManager::USER_EVENT_ID + 0;
	
	const id_t SERVER_CLIENT_CONNECTED = EventManager::USER_EVENT_ID + 1;
	const id_t SERVER_CLIENT_DISCONNECTED = EventManager::USER_EVENT_ID + 2;
	const id_t SERVER_TILE_MOVE_EVENT = EventManager::USER_EVENT_ID + 3;
	const id_t SERVER_PLAYER_WIN_EVENT = EventManager::USER_EVENT_ID + 4;

	struct ServerEvent : public Event
	{
	public:
		byte m0;
		byte m1;
		byte m2;
	};

}