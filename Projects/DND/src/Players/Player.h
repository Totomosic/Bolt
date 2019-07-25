#pragma once
#include "bltpch.h"
#include "../Network/AddressPair.h"
#include "../Map/TilemapLayer.h"

namespace DND
{

	class Player
	{
	private:
		AddressPair m_Address;
		id_t m_PlayerId;
		GameObject* m_PlayerObject;

	public:
		Player();
		Player(AddressPair address, id_t playerId, GameObject* playerObject);

		const AddressPair& Address() const;
		id_t PlayerId() const;
		GameObject* Object() const;

	};

}