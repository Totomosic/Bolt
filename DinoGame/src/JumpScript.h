#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace DinoGame
{

	class JumpScript : public Component
	{
	private:
		float m_RestHeight;
		float m_Gravity;
		float m_Velocity;

	public:
		JumpScript(float gravity);

		void Start() override;
		void Update() override;

		void Jump(float power);
		bool CanJump() const;

		std::unique_ptr<Component> Clone() const override;

	};

}