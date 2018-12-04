#include "UIEventHandler.h"
#include "..\..\Camera.h"
#include "..\MeshRenderer.h"
#include "..\..\..\User\Input.h"

namespace Bolt
{

	UIEventHandler::UIEventHandler() : Component(),
		m_IsHovering(false), OnClicked(Events::ON_CLICKED), OnHoverEntry(Events::ON_HOVER_ENTRY), OnHover(Events::ON_HOVER), OnHoverExit(Events::ON_HOVER_EXIT)
	{
	
	}

	void UIEventHandler::Update()
	{
		bool hovering = IsHovering();
		if (hovering)
		{
			auto args = std::make_unique<UIEvent>();
			args->Object = gameObject();
			OnHover.Post(std::move(args));
		}
		if (!m_IsHovering && hovering)
		{
			auto args = std::make_unique<UIEvent>();
			args->Object = gameObject();
			OnHoverEntry.Post(std::move(args));
		}
		if (m_IsHovering && !hovering)
		{
			auto args = std::make_unique<UIEvent>();
			args->Object = gameObject();
			OnHoverExit.Post(std::move(args));
		}
		if (m_IsHovering && Input::MouseButtonReleased(MouseButton::Left))
		{
			auto args = std::make_unique<UIEvent>();
			args->Object = gameObject();
			OnClicked.Post(std::move(args));
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
				Vector3f mousePosition = Input::MousePosition(viewWidth, viewHeight);
				Cuboid box = renderer.GetMeshBounds();
				return (mousePosition.x > box.Min.x && mousePosition.x < box.Max.x) && (mousePosition.y > box.Min.y && mousePosition.y < box.Max.y);
			}
		}
		return false;
	}

}