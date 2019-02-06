#pragma once
#include "UIelement.h"

namespace Bolt
{

	class BLT_API UIsurface : public UIelement
	{
	protected:
		float m_Width;
		float m_Height;
		Transform m_Transform;
		Material m_Material;

	public:
		UIsurface(float width, float height, const Color& color = Color::White, Transform&& transform = Transform());
		UIsurface(float width, float height, Material material, Transform&& transform = Transform());

		inline float Width() const { return m_Width; }
		inline float Height() const { return m_Height; }

	protected:
		virtual void SetUIroot(UIroot* root) override;

	};

}