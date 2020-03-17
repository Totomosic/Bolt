#include "bltpch.h"
#include "UIEvents.h"

namespace Bolt
{

	UIEvents::UIEvents()
		: m_Bus(std::make_shared<EventBus>()), 
		m_OnClick(m_Bus->GetEmitter<UI<MouseClickEvent>>()),
		m_OnMouseDown(m_Bus->GetEmitter<UI<MouseDownEvent>>()),
		m_OnMouseUp(m_Bus->GetEmitter<UI<MouseUpEvent>>()),
		m_OnKeyDown(m_Bus->GetEmitter<UI<KeyDownEvent>>()),
		m_OnKeyUp(m_Bus->GetEmitter<UI<KeyUpEvent>>()),
		m_OnCharPressed(m_Bus->GetEmitter<UI<CharPressedEvent>>()),
		m_OnFocus(m_Bus->GetEmitter<UI<UIFocus>>()),
		m_OnFocusLost(m_Bus->GetEmitter<UI<UIFocusLost>>())
	{
		m_Bus->SetImmediateMode(true);
	}

	const EventBus& UIEvents::Bus() const
	{
		return *m_Bus;
	}

	EventBus& UIEvents::Bus()
	{
		return *m_Bus;
	}

	const EventEmitter<UI<MouseClickEvent>>& UIEvents::OnClick() const
	{
		return m_OnClick;
	}

	const EventEmitter<UI<MouseDownEvent>>& UIEvents::OnMouseDown() const
	{
		return m_OnMouseDown;
	}

	const EventEmitter<UI<MouseUpEvent>>& UIEvents::OnMouseUp() const
	{
		return m_OnMouseUp;
	}

	const EventEmitter<UI<KeyDownEvent>>& UIEvents::OnKeyDown() const
	{
		return m_OnKeyDown;
	}

	const EventEmitter<UI<KeyUpEvent>>& UIEvents::OnKeyUp() const
	{
		return m_OnKeyUp;
	}

	const EventEmitter<UI<CharPressedEvent>>& UIEvents::OnCharPressed() const
	{
		return m_OnCharPressed;
	}

	const EventEmitter<UI<UIFocus>>& UIEvents::OnFocus() const
	{
		return m_OnFocus;
	}

	const EventEmitter<UI<UIFocusLost>>& UIEvents::OnFocusLost() const
	{
		return m_OnFocusLost;
	}

	EventEmitter<UI<MouseClickEvent>>& UIEvents::OnClick()
	{
		return m_OnClick;
	}

	EventEmitter<UI<MouseDownEvent>>& UIEvents::OnMouseDown()
	{
		return m_OnMouseDown;
	}

	EventEmitter<UI<MouseUpEvent>>& UIEvents::OnMouseUp()
	{
		return m_OnMouseUp;
	}

	EventEmitter<UI<KeyDownEvent>>& UIEvents::OnKeyDown()
	{
		return m_OnKeyDown;
	}

	EventEmitter<UI<KeyUpEvent>>& UIEvents::OnKeyUp()
	{
		return m_OnKeyUp;
	}

	EventEmitter<UI<CharPressedEvent>>& UIEvents::OnCharPressed()
	{
		return m_OnCharPressed;
	}

	EventEmitter<UI<UIFocus>>& UIEvents::OnFocus()
	{
		return m_OnFocus;
	}

	EventEmitter<UI<UIFocusLost>>& UIEvents::OnFocusLost()
	{
		return m_OnFocusLost;
	}

}