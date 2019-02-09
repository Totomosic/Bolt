#pragma once
#include "NetworkManager.h"
#include "Players/PlayerManager.h"

namespace DND
{

	class DndServer
	{
	private:
		PlayerManager* m_Players;

	public:
		DndServer(PlayerManager& players);

		template<typename T>
		void SendPacketToEc2(const T& packet)
		{
			NetworkManager::Get().SendPacket(NetworkManager::Get().EC2Address(), packet);
		}

		template<typename T>
		void SendPacket(const SocketAddress& to, const T& packet)
		{
			NetworkManager::Get().SendPacket(to, packet);
		}

		template<typename T>
		void SendPacket(id_t playerId, const T& packet)
		{
			SendPacket(m_Players->GetPlayer(playerId).Address().PrivateEndpoint, packet);
		}

		template<typename T>
		void SendPacketToAll(const T& packet)
		{
			for (Player& p : m_Players->OtherPlayers())
			{
				BLT_WARN("SENDING PACKET WITH TYPE {} TO {}", (int)T::Type, p.Address().PrivateEndpoint);
				SendPacket(p.PlayerId(), packet);
			}
		}

	};

}