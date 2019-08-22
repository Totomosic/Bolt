#pragma once
#include "Core/Events/EventBus.h"
#include "Core/Events/EventEmitter.h"
#include "Core/Math/Maths.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	// ===========================================================================================================================================
	// EVENT TYPES
	// ===========================================================================================================================================

	class UIElement;

	struct BLT_API UIClickedEvent
	{
	public:
		UIElement& Target;
		Vector2f Position;
		MouseButton Button;
	};

	struct BLT_API UIFocusEvent
	{
	public:
		UIElement& Target;
	};

	struct BLT_API UIFocusLostEvent
	{
	public:
		UIElement& Target;
	};

	struct BLT_API UIMouseDownEvent
	{
	public:
		UIElement& Target;
		Vector2f Position;
		MouseButton Button;
	};

	struct BLT_API UIKeyDownEvent
	{
	public:
		UIElement& Target;
		Keycode Key;
		bool IsRepeat;
	};

	struct BLT_API UIKeyUpEvent
	{
	public:
		UIElement& Target;
		Keycode Key;
	};

	// ===========================================================================================================================================
	// EVENT HANDLER
	// ===========================================================================================================================================

	class BLT_API UIEventHandler
	{
	private:
		EventBus m_Bus;

	public:
		EventEmitter<UIClickedEvent> OnClick;
		EventEmitter<UIFocusEvent> OnFocus;
		EventEmitter<UIFocusLostEvent> OnFocusLost;
		EventEmitter<UIMouseDownEvent> OnMouseDown;
		EventEmitter<UIKeyDownEvent> OnKeyDown;
		EventEmitter<UIKeyUpEvent> OnKeyUp;

	public:
		UIEventHandler();

		const EventBus& Bus() const;
		EventBus& Bus();

	};

}