#include "Entity.h"

namespace RPG
{

	Entity::Entity() : Sprite(),
		m_Velocity(0.0f)
	{

	}

	Entity::Entity(const Vector2f& position, const Vector2f& size, const Texture2D* texture, id_t layerId) : Sprite(position, size, texture, layerId),
		m_Velocity(0.0f)
	{
	
	}

	const Vector2f& Entity::Velocity() const
	{
		return m_Velocity;
	}

	Vector2f& Entity::Velocity()
	{
		return m_Velocity;
	}

	void Entity::Update()
	{
		Sprite::Update();
		Translate(Velocity().x * Time::DeltaTime(), Velocity().y * Time::DeltaTime());
	}

}