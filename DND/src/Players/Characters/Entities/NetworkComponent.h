#pragma once
#include "../../../DndServer.h"

namespace DND
{

	class NetworkComponent : public Component
	{
	private:
		DndServer* m_Server;

	public:
		NetworkComponent(DndServer& server);

		DndServer& Server() const;

		std::unique_ptr<Component> Clone() const override;

	};

}