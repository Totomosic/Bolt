#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace DinoGame
{

	class MotionEngine : public Component
	{
	private:
		const float* m_GameSpeed;
		
	public:
		Vector2f Velocity;

	public:
		MotionEngine(const float* gameSpeed);

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}