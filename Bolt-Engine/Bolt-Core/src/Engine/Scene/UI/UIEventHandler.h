#pragma once
#include "Core/Events/EventBus.h"
#include "Core/Events/EventEmitter.h"
#include "Core/Math/Maths.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	class UIElement;

	struct BLT_API UIClickedEvent
	{
	public:
		UIElement& Element;
		Vector2f Position;
		MouseButton Button;
	};

	struct BLT_API UIFocusEvent
	{
	public:
		UIElement& Element;
	};

	struct BLT_API UIFocusLostEvent
	{
	public:
		UIElement& Element;
	};

	class BLT_API UIEventHandler
	{
	private:
		EventBus m_Bus;

	public:
		EventEmitter<UIClickedEvent> OnClick;
		EventEmitter<UIFocusEvent> OnFocus;
		EventEmitter<UIFocusLostEvent> OnFocusLost;

	public:
		UIEventHandler();

		const EventBus& Bus() const;
		EventBus& Bus();

	};

}