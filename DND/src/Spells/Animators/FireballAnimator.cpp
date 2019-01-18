#include "bltpch.h"
#include "FireballAnimator.h"

namespace DND
{

	FireballAnimator::FireballAnimator(const Vector3f& startingPosition, const Vector3f& target, float timeToTarget, float fireballLength) : Component(),
		m_CurrentTime(0.0f), m_TotalTime(timeToTarget), m_StartPosition(startingPosition), m_Target(target), m_Length(fireballLength)
	{
		
	}

	void FireballAnimator::Start()
	{
		gameObject()->transform().SetLocalPosition(m_StartPosition);
	}

	void FireballAnimator::Update()
	{
		m_CurrentTime += Time::RenderingTimeline().DeltaTime();
		Vector3f toTarget = m_Target - m_StartPosition;
		float length = toTarget.Length() - m_Length;
		gameObject()->transform().SetLocalPosition(m_StartPosition + toTarget.Normalize() * length * m_CurrentTime / m_TotalTime);
		gameObject()->transform().SetLocalOrientation(Quaternion::FromAngleAxis(atan2(toTarget.y, toTarget.x), Vector3f::Forward()));
	}

	std::unique_ptr<Component> FireballAnimator::Clone() const
	{
		return std::make_unique<FireballAnimator>(m_StartPosition, m_Target, m_TotalTime, m_Length);
	}

}