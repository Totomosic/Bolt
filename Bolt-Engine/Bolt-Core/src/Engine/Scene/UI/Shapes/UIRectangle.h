#pragma once
#include "../UIsurface.h"

namespace Bolt
{

	class BLT_API UIRectangle : public UISurface
	{
	private:
		Vector2f m_Size;

	public:
		UIRectangle(UIManager* manager, UIElement* parent, float width, float height, std::unique_ptr<Material>&& material, Transform&& transform);

		const Vector2f& GetSize() const;
		void SetSize(const Vector2f& size);

		bool ContainsPoint(const Vector2f& screenPoint) const override;
	};

}