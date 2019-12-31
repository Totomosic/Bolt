#pragma once
#include "Component.h"
#include "Core/Events/EventBus.h"
#include "Core/Events/EventEmitter.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	class UIElement;

	// Class that defines a UI event wrapper for an input event
	template<typename T>
	struct BLT_API UI
	{
		UIElement& Target;
		T OriginalEvent;
	};

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
		EventEmitter<UI<MouseClickEvent>> m_OnClick;
		EventEmitter<UI<MouseDownEvent>> m_OnMouseDown;
		EventEmitter<UI<MouseUpEvent>> m_OnMouseUp;
		EventEmitter<UI<KeyDownEvent>> m_OnKeyDown;
		EventEmitter<UI<KeyUpEvent>> m_OnKeyUp;
		EventEmitter<UI<CharPressedEvent>> m_OnCharPressed;
		EventEmitter<UI<UIFocus>> m_OnFocus;
		EventEmitter<UI<UIFocusLost>> m_OnFocusLost;

	public:
		UIEvents();

		const EventBus& Bus() const;
		EventBus& Bus();

		const EventEmitter<UI<MouseClickEvent>>& OnClick() const;
		const EventEmitter<UI<MouseDownEvent>>& OnMouseDown() const;
		const EventEmitter<UI<MouseUpEvent>>& OnMouseUp() const;
		const EventEmitter<UI<KeyDownEvent>>& OnKeyDown() const;
		const EventEmitter<UI<KeyUpEvent>>& OnKeyUp() const;
		const EventEmitter<UI<CharPressedEvent>>& OnCharPressed() const;
		const EventEmitter<UI<UIFocus>>& OnFocus() const;
		const EventEmitter<UI<UIFocusLost>>& OnFocusLost() const;

		EventEmitter<UI<MouseClickEvent>>& OnClick();
		EventEmitter<UI<MouseDownEvent>>& OnMouseDown();
		EventEmitter<UI<MouseUpEvent>>& OnMouseUp();
		EventEmitter<UI<KeyDownEvent>>& OnKeyDown();
		EventEmitter<UI<KeyUpEvent>>& OnKeyUp();
		EventEmitter<UI<CharPressedEvent>>& OnCharPressed();
		EventEmitter<UI<UIFocus>>& OnFocus();
		EventEmitter<UI<UIFocusLost>>& OnFocusLost();

	};

}