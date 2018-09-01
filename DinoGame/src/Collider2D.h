#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace DinoGame
{

	class Collider2D : public Component
	{
	private:
		Bolt::Rectangle m_Bounds;
		std::vector<std::function<void(Collider2D*, Collider2D*)>> m_OnCollisionCallbacks;

	public:
		Collider2D(const Bolt::Rectangle& bounds);

		const Bolt::Rectangle& Bounds() const;
		Bolt::Rectangle& Bounds();

		bool CollidesWith(const Collider2D& collider) const;
		void Trigger(Collider2D* collider);
		void Subscribe(const std::function<void(Collider2D*, Collider2D*)>& callback);

		std::unique_ptr<Component> Clone() const override;

	};

}