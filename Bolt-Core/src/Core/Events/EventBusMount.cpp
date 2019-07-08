#include "bltpch.h"
#include "EventBusMount.h"
#include "EventBus.h"

namespace Bolt
{

	EventBusMount::EventBusMount(EventBusBase* bus)
		: m_EventBus(bus), m_Parents(), m_Children()
	{
		
	}

	EventBusMount::~EventBusMount()
	{
		for (EventBusMount* parent : m_Parents)
		{
			parent->RemoveChild(this);
		}
		for (EventBusMount* child : m_Children)
		{
			child->RemoveParent(this);
		}
	}

	EventBusBase* EventBusMount::GetEventBus() const
	{
		return m_EventBus;
	}

	const std::vector<EventBusMount*>& EventBusMount::GetParents() const
	{
		return m_Parents;
	}

	const std::vector<EventBusMount*>& EventBusMount::GetChildren() const
	{
		return m_Children;
	}

	void EventBusMount::AddParent(EventBusMount* bus)
	{
		m_Parents.push_back(bus);
	}

	void EventBusMount::AddChild(EventBusMount* bus)
	{
		m_Children.push_back(bus);
	}

	void EventBusMount::RemoveParent(EventBusMount* bus)
	{
		auto it = std::find(m_Parents.begin(), m_Parents.end(), bus);
		if (it != m_Parents.end())
		{
			m_Parents.erase(it);
		}
	}

	void EventBusMount::RemoveChild(EventBusMount* bus)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), bus);
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
	}

	void EventBusMount::SetEventBus(EventBusBase* bus)
	{
		m_EventBus = bus;
	}

}