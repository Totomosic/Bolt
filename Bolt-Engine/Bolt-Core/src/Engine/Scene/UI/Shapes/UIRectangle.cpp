#include "bltpch.h"
#include "UIRectangle.h"
#include "../UIManager.h"

namespace Bolt
{

	UIRectangle::UIRectangle(UIManager* manager, UIElement* parent, float width, float height, std::unique_ptr<Material>&& material, Transform&& transform) : UISurface(manager, parent),
		m_Size(width, height)
	{
		SetGameObject(m_Manager->Factory().Rectangle(width, height, std::move(material), std::move(transform)));
	}

	const Bolt::Vector2f& UIRectangle::GetSize() const
	{
		return m_Size;
	}

	void UIRectangle::SetSize(const Vector2f& size)
	{
		Mesh& mesh = GetGameObject()->mesh().Mesh;
		mesh.Models[0].Transform *= Matrix4f::Scale(size / m_Size);
		m_Size = size;
	}

	bool UIRectangle::ContainsPoint(const Vector2f& screenPoint) const
	{
		Vector3f position = GetGameObject()->transform().Position();
		return (screenPoint.x >= position.x - GetSize().x / 2.0f && screenPoint.x <= position.x + GetSize().x / 2.0f)
			&& (screenPoint.y >= position.y - GetSize().y / 2.0f && screenPoint.y <= position.y + GetSize().y / 2.0f);
	}

}