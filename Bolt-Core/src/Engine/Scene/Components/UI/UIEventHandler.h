#pragma once
#include "..\Component.h"
#include "..\..\GameObject.h"

namespace Bolt
{

	struct BLT_API UIEventArgs : public EventArgs
	{
	public:
		GameObject* Object;
	};

	class BLT_API UIEventHandler : public Component
	{
	private:
		bool m_IsHovering;

	public:
		EventDispatcher<UIEventArgs> OnClicked;
		EventDispatcher<UIEventArgs> OnHoverEntry;
		EventDispatcher<UIEventArgs> OnHover;
		EventDispatcher<UIEventArgs> OnHoverExit;

	public:
		UIEventHandler();

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}