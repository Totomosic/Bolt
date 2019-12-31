#include "bltpch.h"
#include "UIRectangle.h"

namespace Bolt
{

	UIRectangle::UIRectangle(UIManager* manager, UIElement* parent, float width, float height, std::unique_ptr<Material>&& material, Transform transform)
		: UIElement(manager, parent), m_Width(width), m_Height(height)
	{
		EntityHandle entity = GetEntityFactory().Rectangle(width, height, std::move(material), std::move(transform));
		SetEntity(entity);
	}

	float UIRectangle::GetWidth() const
	{
		return m_Width;
	}

	float UIRectangle::GetHeight() const
	{
		return m_Height;
	}

	bool UIRectangle::ContainsPoint(const Vector2f& point) const
	{
		Vector3f position = GetTransform().Position();
		Vector3f min = position + Vector3f(-m_Width / 2.0f, -m_Height / 2.0f, 0.0f);
		Vector3f max = position + Vector3f(m_Width / 2.0f, m_Height / 2.0f, 0.0f);
		return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
	}

}