#include "bltpch.h"
#include "UISurface.h"

#include "UIManager.h"

namespace Bolt
{

	UISurface::UISurface(UIManager* manager, UIElement* parent, float width, float height, std::unique_ptr<Material>&& material, Transform&& transform) : UIElement(manager, parent)
	{
		SetGameObject(m_Manager->Factory().Rectangle(width, height, std::move(material), std::move(transform)));
	}

}