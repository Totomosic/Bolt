#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace WaterShowcase
{

	class PlayerController : public Component
	{
	public:
		float Speed;

	public:
		PlayerController(float speed);
		PlayerController();

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}