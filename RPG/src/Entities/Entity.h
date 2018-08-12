#pragma once
#include "Sprite.h"

namespace RPG
{

	class Entity : public Sprite
	{
	protected:
		Vector2f m_Velocity;

	public:
		Entity();
		Entity(const Vector2f& position, const Vector2f& size, const Texture2D* texture, id_t layerId = 0);

		const Vector2f& Velocity() const;
		Vector2f& Velocity();

		void Update() override;

	};

}