#include "bltpch.h"

#include "UIEventHandler.h"
#include "../../Camera.h"
#include "../MeshRenderer.h"
#include "Engine/User/Input.h"

namespace Bolt
{

	UIEventHandler::UIEventHandler() : Component(),
		m_IsHovering(false), m_EventBus(),
		OnClicked(m_EventBus.GetEmitter<UIClickedEvent>(Events::ON_CLICKED)), 
		OnHoverEntry(m_EventBus.GetEmitter<UIHoverEntryEvent>(Events::ON_HOVER_ENTRY)),
		OnHover(m_EventBus.GetEmitter<UIHoverEvent>(Events::ON_HOVER)),
		OnHoverExit(m_EventBus.GetEmitter<UIHoverExitEvent>(Events::ON_HOVER_EXIT))
	{
	
	}

	void UIEventHandler::Update()
	{
		bool hovering = IsHovering();
		if (hovering)
		{
			UIHoverEvent args;
			args.Object = gameObject();
			args.ScreenPosition = Input::Get().MousePosition().xy();
			args.ObjectRelPosition = args.ScreenPosition - args.Object->transform().Position().xy();
			OnHover.Emit(std::move(args));
		}
		if (!m_IsHovering && hovering)
		{
			UIHoverEntryEvent args;
			args.Object = gameObject();
			args.ScreenPosition = Input::Get().MousePosition().xy();
			args.ObjectRelPosition = args.ScreenPosition - args.Object->transform().Position().xy();
			OnHoverEntry.Emit(std::move(args));
		}
		if (m_IsHovering && !hovering)
		{
			UIHoverExitEvent args;
			args.Object = gameObject();
			args.ScreenPosition = Input::Get().MousePosition().xy();
			args.ObjectRelPosition = args.ScreenPosition - args.Object->transform().Position().xy();
			OnHoverExit.Emit(std::move(args));
		}
		if (m_IsHovering)
		{
			int button = (Input::Get().MouseButtonReleased(MouseButton::Left)) ? (int)MouseButton::Left : (Input::Get().MouseButtonReleased(MouseButton::Right)) ? (int)MouseButton::Right : (Input::Get().MouseButtonDown(MouseButton::Middle)) ? (int)MouseButton::Middle : -1;
			if (button != -1)
			{
				UIClickedEvent args;
				args.Object = gameObject();
				args.ScreenPosition = Input::Get().MousePosition().xy();
				args.ObjectRelPosition = args.ScreenPosition - args.Object->transform().Position().xy();
				args.Button = (MouseButton)button;
				OnClicked.Emit(std::move(args));
			}
		}
		m_IsHovering = hovering;
	}

	std::unique_ptr<Component> UIEventHandler::Clone() const
	{
		return std::make_unique<UIEventHandler>();
	}

	bool UIEventHandler::IsHovering() const
	{
		if (gameObject()->Components().HasComponent<MeshRenderer>())
		{
			if (camera()->CameraProjection().Type == ProjectionType::Orthographic)
			{
				MeshRenderer& renderer = gameObject()->Components().GetComponent<MeshRenderer>();
				float viewWidth = camera()->ViewWidth();
				float viewHeight = camera()->ViewHeight();
				Vector3f mousePosition = Input::Get().MousePosition(viewWidth, viewHeight) + camera()->transform().Position();
				std::vector<Cuboid> bounds = renderer.GetMeshBounds();
				for (Cuboid& box : bounds)
				{
					bool hit = (mousePosition.x > box.Min.x && mousePosition.x < box.Max.x) && (mousePosition.y > box.Min.y && mousePosition.y < box.Max.y);
					if (hit)
					{
						return hit;
					}
				}
				return false;
			}
		}
		return false;
	}

}