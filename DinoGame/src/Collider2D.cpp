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
		Vector2f myPos = gameObject()->transform().Position().xy();
		Vector2f otherPos = collider.gameObject()->transform().Position().xy();
		float myW = Bounds().Width() / 2.0f;
		float myH = Bounds().Height() / 2.0f;
		float otherW = collider.Bounds().Width() / 2.0f;
		float otherH = collider.Bounds().Height() / 2.0f;

		float myLeft = myPos.x - myW;
		float myRight = myPos.x + myW;
		float otherLeft = otherPos.x - otherW;
		float otherRight = otherPos.x + otherW;
		float myBottom = myPos.y - myH;
		float myTop = myPos.y + myH;
		float otherBottom = otherPos.y - otherH;
		float otherTop = otherPos.y + otherH;

		bool x = (myLeft <= otherRight && myLeft >= otherLeft) || (myRight >= otherLeft && myRight <= otherRight) || (myLeft <= otherLeft && myRight >= otherRight);
		bool y = (myBottom <= otherTop && myBottom >= otherBottom) || (myTop <= otherTop && myTop >= otherBottom) || (myTop >= otherTop && myBottom <= otherBottom);
		return x && y;
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