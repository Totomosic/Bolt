#pragma once
#include "bltpch.h"
#include "NetworkPacketSerialization.h"

namespace DND
{

	class ConnectionManager
	{
	private:
		IdManager<id_t> m_ConnectionIdManager;
		std::unordered_map<id_t, AddressPair> m_Connections;
		std::unordered_map<id_t, SocketAddress> m_RoutableAddresses;

	public:
		ConnectionManager();

		const AddressPair& GetAddressPair(id_t connectionId) const;
		const SocketAddress& GetRoutableAddress(id_t connectionId) const;
		const std::unordered_map<id_t, AddressPair>& Connections() const;
		id_t GetConnectionId(const AddressPair& address) const;
		std::vector<SocketAddress> GetAllRoutableAddresses() const;

		id_t AddConnection(const AddressPair& address, const SocketAddress& routableAddress);
		void RemoveConnection(id_t connectionId);
		void ClearConnections();

	};

}