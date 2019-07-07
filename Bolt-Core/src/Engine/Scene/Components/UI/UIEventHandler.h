#pragma once
#include "../Component.h"
#include "Engine/Scene/GameObject.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	class BLT_API UIClickedEvent
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;
		MouseButton Button;
	};

	class BLT_API UIHoverEvent
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;
	};

	class BLT_API UIHoverEntryEvent
	{
	public:
		GameObject* Object;
		Vector2f ScreenPosition;
		Vector2f ObjectRelPosition;
	};

	class BLT_API UIHoverExitEvent
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
		EventBus m_EventBus;

	public:
		EventEmitter<UIClickedEvent> OnClicked;
		EventEmitter<UIHoverEntryEvent> OnHoverEntry;
		EventEmitter<UIHoverEvent> OnHover;
		EventEmitter<UIHoverExitEvent> OnHoverExit;

	public:
		UIEventHandler();

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}