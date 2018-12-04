#pragma once
#include "..\Component.h"
#include "..\..\GameObject.h"

namespace Bolt
{

	struct BLT_API UIEvent : public Event
	{
	public:
		GameObject* Object;
	};

	class BLT_API UIEventHandler : public Component
	{
	private:
		bool m_IsHovering;

	public:
		EventDispatcher<UIEvent> OnClicked;
		EventDispatcher<UIEvent> OnHoverEntry;
		EventDispatcher<UIEvent> OnHover;
		EventDispatcher<UIEvent> OnHoverExit;

	public:
		UIEventHandler();

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}