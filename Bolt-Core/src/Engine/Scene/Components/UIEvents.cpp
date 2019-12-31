#include "bltpch.h"
#include "UIEvents.h"
#include "Core/Events/Events.h"

namespace Bolt
{

	UIEvents::UIEvents()
		: m_Bus(std::make_shared<EventBus>()), 
		m_OnClick(m_Bus->GetEmitter<MouseClickEvent>(Events::UI.OnClick)),
		m_OnMouseDown(m_Bus->GetEmitter<MouseDownEvent>(Events::UI.OnMouseDown)),
		m_OnMouseUp(m_Bus->GetEmitter<MouseUpEvent>(Events::UI.OnMouseUp)),
		m_OnKeyDown(m_Bus->GetEmitter<KeyDownEvent>(Events::UI.OnKeyDown)),
		m_OnKeyUp(m_Bus->GetEmitter<KeyUpEvent>(Events::UI.OnKeyUp)),
		m_OnCharPressed(m_Bus->GetEmitter<CharPressedEvent>(Events::UI.OnCharPressed)),
		m_OnFocus(m_Bus->GetEmitter<UIFocus>(Events::UI.OnFocus)),
		m_OnFocusLost(m_Bus->GetEmitter<UIFocusLost>(Events::UI.OnFocusLost))
	{
		m_Bus->SetImmediateMode(true);
		// Stop focus events from propagating
		m_OnFocus.AddEventListener([](Event<UIFocus>& e)
			{
				e.StopPropagation();
			}, ListenerPriority::Low);
		m_OnFocusLost.AddEventListener([](Event<UIFocusLost>& e)
			{
				e.StopPropagation();
			}, ListenerPriority::Low);
	}

	const EventBus& UIEvents::Bus() const
	{
		return *m_Bus;
	}

	EventBus& UIEvents::Bus()
	{
		return *m_Bus;
	}

	const EventEmitter<MouseClickEvent>& UIEvents::OnClick() const
	{
		return m_OnClick;
	}

	const EventEmitter<MouseDownEvent>& UIEvents::OnMouseDown() const
	{
		return m_OnMouseDown;
	}

	const EventEmitter<MouseUpEvent>& UIEvents::OnMouseUp() const
	{
		return m_OnMouseUp;
	}

	const EventEmitter<KeyDownEvent>& UIEvents::OnKeyDown() const
	{
		return m_OnKeyDown;
	}

	const EventEmitter<KeyUpEvent>& UIEvents::OnKeyUp() const
	{
		return m_OnKeyUp;
	}

	const EventEmitter<CharPressedEvent>& UIEvents::OnCharPressed() const
	{
		return m_OnCharPressed;
	}

	const EventEmitter<UIFocus>& UIEvents::OnFocus() const
	{
		return m_OnFocus;
	}

	const EventEmitter<UIFocusLost>& UIEvents::OnFocusLost() const
	{
		return m_OnFocusLost;
	}

	EventEmitter<MouseClickEvent>& UIEvents::OnClick()
	{
		return m_OnClick;
	}

	EventEmitter<MouseDownEvent>& UIEvents::OnMouseDown()
	{
		return m_OnMouseDown;
	}

	EventEmitter<MouseUpEvent>& UIEvents::OnMouseUp()
	{
		return m_OnMouseUp;
	}

	EventEmitter<KeyDownEvent>& UIEvents::OnKeyDown()
	{
		return m_OnKeyDown;
	}

	EventEmitter<KeyUpEvent>& UIEvents::OnKeyUp()
	{
		return m_OnKeyUp;
	}

	EventEmitter<CharPressedEvent>& UIEvents::OnCharPressed()
	{
		return m_OnCharPressed;
	}

	EventEmitter<UIFocus>& UIEvents::OnFocus()
	{
		return m_OnFocus;
	}

	EventEmitter<UIFocusLost>& UIEvents::OnFocusLost()
	{
		return m_OnFocusLost;
	}

}