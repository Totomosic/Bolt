#include "bltpch.h"
#include "Player.h"

namespace DND
{

	Player::Player()
	{
	
	}

	Player::Player(AddressPair address, id_t playerId, GameObject* playerObject)
		: m_Address(std::move(address)), m_PlayerId(playerId), m_PlayerObject(playerObject)
	{
	
	}

	const AddressPair& Player::Address() const
	{
		return m_Address;
	}

	id_t Player::PlayerId() const
	{
		return m_PlayerId;
	}

	GameObject* Player::Object() const
	{
		return m_PlayerObject;
	}

}