#pragma once
#include "..\Component.h"
#include "..\..\GameObject.h"
#include "..\..\..\User\Input.h"

namespace Bolt
{

	class BLT_API UIClickedEvent : public Event
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;
		MouseButton Button;
	};

	class BLT_API UIHoverEvent : public Event
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;
	};

	class BLT_API UIEventHandler : public Component
	{
	private:
		bool m_IsHovering;

	public:
		EventDispatcher<UIClickedEvent> OnClicked;
		EventDispatcher<UIHoverEvent> OnHoverEntry;
		EventDispatcher<UIHoverEvent> OnHover;
		EventDispatcher<UIHoverEvent> OnHoverExit;

	public:
		UIEventHandler();

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}