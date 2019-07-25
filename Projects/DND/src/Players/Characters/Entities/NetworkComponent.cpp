#include "bltpch.h"
#include "NetworkComponent.h"

namespace DND
{
	
	NetworkComponent::NetworkComponent(DndServer& server) : Component(),
		m_Server(&server)
	{
	
	}

	DndServer& NetworkComponent::Server() const
	{
		return *m_Server;
	}

	std::unique_ptr<Component> NetworkComponent::Clone() const
	{
		return std::make_unique<NetworkComponent>(Server());
	}

}