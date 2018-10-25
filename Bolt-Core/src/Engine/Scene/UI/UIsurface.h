#pragma once
#include "UIelement.h"

namespace Bolt
{

	class BLT_API UIsurface : public UIelement
	{
	private:
		Vector2f m_Size;
		Material m_Material;
		Transform m_Transform;

	public:
		UIsurface(const Vector2f& size = Vector2f(0.0f, 0.0f), Transform&& transform = Transform());
		UIsurface(const Vector2f& size, Material material, Transform&& transform = Transform());
		UIsurface(const Vector2f& size, const Color& color, Transform&& transform = Transform());
		UIsurface(const Vector2f& size, const ResourcePtr<const Texture2D>& texture, Transform&& transform = Transform());

		const Vector2f& Size() const;
		const Material& GetMaterial() const;
		
		void SetSize(const Vector2f& size);
		void SetMaterial(Material material);

	protected:
		void CreateGameObject() override;

	};

}