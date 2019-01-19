#pragma once
#include "bltpch.h"

namespace DND
{

	class FireballAnimator : public Component
	{
	private:
		float m_CurrentTime;
		float m_TotalTime;
		Vector3f m_StartPosition;
		Vector2f m_ToTargetDir;
		float m_Length;

	public:
		FireballAnimator(const Vector3f& startPosition, const Vector3f& target, float timeToTarget, float fireballLength);

		void Start() override;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}