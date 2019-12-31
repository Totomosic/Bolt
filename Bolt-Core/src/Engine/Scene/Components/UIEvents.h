#pragma once
#include "Component.h"
#include "Core/Events/EventBus.h"
#include "Core/Events/EventEmitter.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	struct BLT_API UIFocus
	{

	};

	struct BLT_API UIFocusLost
	{

	};
	
	class BLT_API UIEvents : public Component<UIEvents>
	{
	private:
		std::shared_ptr<EventBus> m_Bus;
		EventEmitter<MouseClickEvent> m_OnClick;
		EventEmitter<MouseDownEvent> m_OnMouseDown;
		EventEmitter<MouseUpEvent> m_OnMouseUp;
		EventEmitter<KeyDownEvent> m_OnKeyDown;
		EventEmitter<KeyUpEvent> m_OnKeyUp;
		EventEmitter<CharPressedEvent> m_OnCharPressed;
		EventEmitter<UIFocus> m_OnFocus;
		EventEmitter<UIFocusLost> m_OnFocusLost;

	public:
		UIEvents();

		const EventBus& Bus() const;
		EventBus& Bus();

		const EventEmitter<MouseClickEvent>& OnClick() const;
		const EventEmitter<MouseDownEvent>& OnMouseDown() const;
		const EventEmitter<MouseUpEvent>& OnMouseUp() const;
		const EventEmitter<KeyDownEvent>& OnKeyDown() const;
		const EventEmitter<KeyUpEvent>& OnKeyUp() const;
		const EventEmitter<CharPressedEvent>& OnCharPressed() const;
		const EventEmitter<UIFocus>& OnFocus() const;
		const EventEmitter<UIFocusLost>& OnFocusLost() const;

		EventEmitter<MouseClickEvent>& OnClick();
		EventEmitter<MouseDownEvent>& OnMouseDown();
		EventEmitter<MouseUpEvent>& OnMouseUp();
		EventEmitter<KeyDownEvent>& OnKeyDown();
		EventEmitter<KeyUpEvent>& OnKeyUp();
		EventEmitter<CharPressedEvent>& OnCharPressed();
		EventEmitter<UIFocus>& OnFocus();
		EventEmitter<UIFocusLost>& OnFocusLost();

	};

}