#include "MouseHoverable.h"

namespace Bolt
{

	MouseHoverable::MouseHoverable() : MouseHoverable(MouseHoverable::HoverFunc())
	{
	
	}

	MouseHoverable::MouseHoverable(MouseHoverable::HoverFunc onEntry, MouseHoverable::HoverFunc onHover, MouseHoverable::HoverFunc onExit) : Component(),
		OnHoverEntry(onEntry), OnHover(onHover), OnHoverExit(onExit), IsHovered(false)
	{
	
	}

	void MouseHoverable::Update()
	{
		bool hovering = IsHovering();
		bool wasHovering = IsHovered;
		IsHovered = hovering;
		if (OnHoverEntry && !wasHovering && hovering)
		{
			OnHoverEntry(*this);
		}
		if (OnHover && hovering)
		{
			OnHover(*this);
		}
		if (OnHoverExit && wasHovering && !hovering)
		{
			OnHoverExit(*this);
		}
	}

	std::unique_ptr<Component> MouseHoverable::Clone() const
	{
		return std::make_unique<MouseHoverable>(OnHoverEntry, OnHover, OnHoverExit);
	}

	bool MouseHoverable::IsHovering() const
	{
		return false;
	}

}