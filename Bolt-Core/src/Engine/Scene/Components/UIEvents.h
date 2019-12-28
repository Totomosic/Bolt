#pragma once
#include "Component.h"
#include "Core/Events/EventBus.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	struct BLT_API UIClick
	{
	public:
		int x;
		int y;
		float RelX;
		float RelY;
		MouseButton Button;
	};

	struct BLT_API UIMouseDown
	{
	public:
		int x;
		int y;
		float RelX;
		float RelY;
		MouseButton Button;
	};

	struct BLT_API UIMouseUp
	{
	public:
		int x;
		int y;
		float RelX;
		float RelY;
		MouseButton Button;
	};
	
	class BLT_API UIEvents : public Component<UIEvents>
	{
	private:
		EventBus m_Bus;
		EventEmitter<UIClick> m_OnClick;
		EventEmitter<UIMouseDown> m_OnMouseDown;
		EventEmitter<UIMouseUp> m_OnMouseUp;

	public:
		UIEvents();

		const EventBus& Bus() const;
		EventBus& Bus();

		const EventEmitter<UIClick>& OnClick() const;
		const EventEmitter<UIMouseDown>& OnMouseDown() const;
		const EventEmitter<UIMouseUp>& OnMouseUp() const;
	};

}