#include "bltpch.h"
#include "UIEventHandler.h"

namespace Bolt
{

	UIEventHandler::UIEventHandler()
		: m_Bus(), OnClick(m_Bus.GetEmitter<UIClickedEvent>(Events::UI.OnClicked)), OnFocus(m_Bus.GetEmitter<UIFocusEvent>(Events::UI.OnFocus)), OnFocusLost(m_Bus.GetEmitter<UIFocusLostEvent>(Events::UI.OnFocusLost)),
		OnMouseDown(m_Bus.GetEmitter<UIMouseDownEvent>(Events::UI.OnMouseDown)), OnKeyDown(m_Bus.GetEmitter<UIKeyDownEvent>(Events::UI.OnKeyDown)), OnKeyUp(m_Bus.GetEmitter<UIKeyUpEvent>(Events::UI.OnKeyUp))
	{
		
	}

	const EventBus& UIEventHandler::Bus() const
	{
		return m_Bus;
	}

	EventBus& UIEventHandler::Bus()
	{
		return m_Bus;
	}

}