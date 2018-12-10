#include "Types.h"
#include "ScrollAnimator.h"

namespace Bolt
{

	ScrollAnimator::ScrollAnimator(const Vector2f& scrollVector, float speed) : TextureAnimator(),
		m_ScrollVector(scrollVector), m_ScrollSpeed(speed), m_Center(0.5f, 0.5f)
	{
	
	}

	float ScrollAnimator::ScrollSpeed() const
	{
		return m_ScrollSpeed;
	}

	float& ScrollAnimator::ScrollSpeed()
	{
		return m_ScrollSpeed;
	}

	void ScrollAnimator::Update(double elapsedSeconds)
	{
		m_Center -= m_ScrollVector * m_ScrollSpeed * elapsedSeconds;
	}

	TextureBounds ScrollAnimator::GetBounds() const
	{
		TextureBounds bounds;
		bounds.MinX = m_Center.x - 0.5f;
		bounds.MaxX = m_Center.x + 0.5f;
		bounds.MinY = m_Center.y - 0.5f;
		bounds.MaxY = m_Center.y + 0.5f;
		return bounds;
	}

	std::unique_ptr<TextureAnimator> ScrollAnimator::Clone() const
	{
		return std::make_unique<ScrollAnimator>(m_ScrollVector, m_ScrollSpeed);
	}

}