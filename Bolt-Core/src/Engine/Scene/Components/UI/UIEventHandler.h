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

		BLT_EVENT_ID_DEF(Events::ON_CLICKED);
	};

	class BLT_API UIHoverEvent : public Event
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;

		BLT_EVENT_ID_DEF(Events::ON_HOVER);
	};

	class BLT_API UIHoverEntryEvent : public Event
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;

		BLT_EVENT_ID_DEF(Events::ON_HOVER_ENTRY);
	};

	class BLT_API UIHoverExitEvent : public Event
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;

		BLT_EVENT_ID_DEF(Events::ON_HOVER_ENTRY);
	};

	class BLT_API UIEventHandler : public Component
	{
	private:
		bool m_IsHovering;

	public:
		EventDispatcher<UIClickedEvent> OnClicked;
		EventDispatcher<UIHoverEntryEvent> OnHoverEntry;
		EventDispatcher<UIHoverEvent> OnHover;
		EventDispatcher<UIHoverExitEvent> OnHoverExit;

	public:
		UIEventHandler();

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}