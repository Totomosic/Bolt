#include "bltpch.h"
#include "UIEvents.h"
#include "Core/Events/Events.h"

namespace Bolt
{

	UIEvents::UIEvents()
		: m_Bus(), 
		m_OnClick(m_Bus.GetEmitter<UIClick>(Events::UI.OnClick)),
		m_OnMouseDown(m_Bus.GetEmitter<UIMouseDown>(Events::UI.OnMouseDown)),
		m_OnMouseUp(m_Bus.GetEmitter<UIMouseUp>(Events::UI.OnMouseUp))
	{
	}

	const EventBus& UIEvents::Bus() const
	{
		return m_Bus;
	}

	EventBus& UIEvents::Bus()
	{
		return m_Bus;
	}

	const EventEmitter<UIClick>& UIEvents::OnClick() const
	{
		return m_OnClick;
	}

	const EventEmitter<UIMouseDown>& UIEvents::OnMouseDown() const
	{
		return m_OnMouseDown;
	}

	const EventEmitter<UIMouseUp>& UIEvents::OnMouseUp() const
	{
		return m_OnMouseUp;
	}

}