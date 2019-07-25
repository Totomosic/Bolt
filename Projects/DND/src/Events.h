#pragma once
#include "bltpch.h"

namespace DND
{

	constexpr int PACKET_RECEIVED_EVENT = EventManager::USER_EVENT_ID + 0;
	constexpr int SERVER_SHUTDOWN_EVENT = EventManager::USER_EVENT_ID + 1;

	constexpr int START_GAME_EVENT = EventManager::USER_EVENT_ID + 50;

}