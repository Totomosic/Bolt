#pragma once
#include "../TextureAnimator.h"

namespace Bolt
{

	class BLT_API ScrollAnimator : public TextureAnimator
	{
	private:
		Vector2f m_ScrollVector;
		float m_ScrollSpeed;
		Vector2f m_Center;

	public:
		ScrollAnimator(const Vector2f& scrollVector, float speed);

		float ScrollSpeed() const;
		float& ScrollSpeed();

		void Update(double elapsedSeconds) override;
		TextureBounds GetBounds() const override;
		std::unique_ptr<TextureAnimator> Clone() const override;

	};

}