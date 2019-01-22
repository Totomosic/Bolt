#include "bltpch.h"
#include "FireballAnimator.h"

namespace DND
{

	FireballAnimator::FireballAnimator(const Vector3f& startingPosition, const Vector3f& target, float timeToTarget, float fireballLength) : Component(),
		m_CurrentTime(0.0f), m_TotalTime(timeToTarget), m_StartPosition(startingPosition), m_ToTargetDir(target.xy() - startingPosition.xy()), m_Length(m_ToTargetDir.Length() - fireballLength)
	{
		m_ToTargetDir = m_ToTargetDir.Normalize();
	}

	void FireballAnimator::Start()
	{
		gameObject()->transform().SetLocalPosition(m_StartPosition);
		gameObject()->transform().SetLocalOrientation(Quaternion::FromAngleAxis(atan2(m_ToTargetDir.y, m_ToTargetDir.x), Vector3f::Forward()));
	}

	void FireballAnimator::Update()
	{
		m_CurrentTime += Time::RenderingTimeline().DeltaTime();
		gameObject()->transform().SetLocalPosition(m_StartPosition + Vector3f(m_ToTargetDir, 0) * m_Length * m_CurrentTime / m_TotalTime);
	}

	std::unique_ptr<Component> FireballAnimator::Clone() const
	{
		BLT_ASSERT(false, "UNABLE TO CLONE FIREBALL ANIMATOR");
		return std::make_unique<FireballAnimator>(m_StartPosition, Vector3f(), m_TotalTime, m_Length);
	}

}