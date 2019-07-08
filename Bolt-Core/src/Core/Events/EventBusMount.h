#pragma once
#include "bltpch.h"

namespace Bolt
{

	class EventBusBase;

	class BLT_API EventBusMount
	{
	private:
		EventBusBase* m_EventBus;
		std::vector<EventBusMount*> m_Parents;
		std::vector<EventBusMount*> m_Children;

	public:
		EventBusMount(EventBusBase* eventBus);
		EventBusMount(const EventBusMount& other) = delete;
		EventBusMount& operator=(const EventBusMount& other) = delete;
		EventBusMount(EventBusMount&& other) = delete;
		EventBusMount& operator=(EventBusMount&& other) = delete;
		~EventBusMount();

		EventBusBase* GetEventBus() const;
		const std::vector<EventBusMount*>& GetParents() const;
		const std::vector<EventBusMount*>& GetChildren() const;

		void AddParent(EventBusMount* bus);
		void AddChild(EventBusMount* bus);
		void RemoveParent(EventBusMount* bus);
		void RemoveChild(EventBusMount* bus);
		void SetEventBus(EventBusBase* bus);

	};

}