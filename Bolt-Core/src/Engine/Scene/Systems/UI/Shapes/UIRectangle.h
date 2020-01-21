#pragma once
#include "../UIElement.h"

namespace Bolt
{

	class BLT_API UIRectangle : public UIElement
	{
	private:
		float m_Width;
		float m_Height;

	public:
		UIRectangle(UIManager* manager, UIElement* parent, float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform());

		float GetWidth() const;
		float GetHeight() const;

		virtual bool ContainsPoint(const Vector2f& point) const override;
	};

}