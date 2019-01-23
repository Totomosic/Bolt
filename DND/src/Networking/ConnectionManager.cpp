#include "bltpch.h"
#include "ConnectionManager.h"

namespace DND
{

	ConnectionManager::ConnectionManager()
		: m_ConnectionIdManager(0, GameObject::InvalidID - 1), m_Connections(), m_RoutableAddresses()
	{
	
	}

	const AddressPair& ConnectionManager::GetAddressPair(id_t connectionId) const
	{
		return m_Connections.at(connectionId);
	}

	const SocketAddress& ConnectionManager::GetRoutableAddress(id_t connectionId) const
	{
		return m_RoutableAddresses.at(connectionId);
	}

	const std::unordered_map<id_t, AddressPair>& ConnectionManager::Connections() const
	{
		return m_Connections;
	}

	id_t ConnectionManager::GetConnectionId(const AddressPair& address) const
	{
		for (const auto& pair : m_Connections)
		{
			if (pair.second.PublicEndpoint == address.PublicEndpoint || pair.second.PrivateEndpoint == address.PrivateEndpoint)
			{
				return pair.first;
			}
		}
		return GameObject::InvalidID;
	}

	std::vector<SocketAddress> ConnectionManager::GetAllRoutableAddresses() const
	{
		std::vector<SocketAddress> result;
		for (const auto& pair : m_Connections)
		{
			result.push_back(GetRoutableAddress(pair.first));
		}
		return result;
	}

	id_t ConnectionManager::AddConnection(const AddressPair& address, const SocketAddress& routableAddress)
	{
		id_t connectionId = m_ConnectionIdManager.GetNextId();
		m_Connections[connectionId] = address;
		m_RoutableAddresses[connectionId] = routableAddress;
		return connectionId;
	}

	void ConnectionManager::RemoveConnection(id_t connectionId)
	{
		m_Connections.erase(connectionId);
		m_RoutableAddresses.erase(connectionId);
		m_ConnectionIdManager.ReleaseId(connectionId);
	}

	void ConnectionManager::ClearConnections()
	{
		m_Connections.clear();
		m_RoutableAddresses.clear();
		m_ConnectionIdManager.Reset();
	}

}