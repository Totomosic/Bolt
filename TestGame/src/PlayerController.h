#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace MeteorGame
{

	class PlayerController : public Component
	{
	public:
		float Speed;

	public:
		PlayerController();

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}