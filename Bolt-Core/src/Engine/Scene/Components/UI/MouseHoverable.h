#pragma once
#include "..\Component.h"
#include "..\..\GameObject.h"

namespace Bolt
{

	class BLT_API MouseHoverable : public Component
	{
	public:
		using HoverFunc = std::function<void(MouseHoverable&)>;

	public:
		HoverFunc OnHoverEntry;
		HoverFunc OnHover;
		HoverFunc OnHoverExit;
		bool IsHovered;

	public:
		MouseHoverable();
		MouseHoverable(HoverFunc onEntry, HoverFunc onHover = HoverFunc(), HoverFunc onExit = HoverFunc());

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	private:
		bool IsHovering() const;

	};

}