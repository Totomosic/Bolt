#include "Types.h"
#include "UIroot.h"
#include "..\Layer.h"

namespace Bolt
{

	UIroot::UIroot(Layer* layer, Transform&& transform) : UIelement(),
		m_Layer(layer)
	{
		SetGameObject(GameObject::Instantiate(layer, std::move(transform)));
	}

	UIroot::UIroot(Layer* layer, GameObject* object)
		: m_Layer(layer)
	{
		SetGameObject(object);
	}

	Layer* UIroot::GetLayer() const
	{
		return m_Layer;
	}

	void UIroot::SetAnchorTransform(Transform&& anchorPosition)
	{
		m_GameObject->transform() = std::move(anchorPosition);
	}


}