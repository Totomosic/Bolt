#include "Collider2D.h"

namespace DinoGame
{

	Collider2D::Collider2D(const Bolt::Rectangle& bounds) : Component(),
		m_Bounds(bounds), m_OnCollisionCallbacks()
	{
	
	}

	const Bolt::Rectangle& Collider2D::Bounds() const
	{
		return m_Bounds;
	}

	Bolt::Rectangle& Collider2D::Bounds()
	{
		return m_Bounds;
	}

	bool Collider2D::CollidesWith(const Collider2D& collider) const
	{
	
	}

	void Collider2D::Trigger(Collider2D* collider)
	{
		for (auto& func : m_OnCollisionCallbacks)
		{
			func(collider, this);
		}
	}

	void Collider2D::Subscribe(const std::function<void(Collider2D*, Collider2D*)>& callback)
	{
		m_OnCollisionCallbacks.push_back(callback);
	}

	std::unique_ptr<Component> Collider2D::Clone() const
	{
		return std::make_unique<Collider2D>(m_Bounds);
	}

}