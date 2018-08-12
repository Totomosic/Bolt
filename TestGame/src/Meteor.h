#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace MeteorGame
{

	enum class MeteorType
	{
		Default,
		Flame
	};

	class Meteor : public Component
	{
	public:
		float Radius;
		bool IsBroken;
		MeteorType Type;

	public:
		Meteor(float radius, MeteorType type);

		void Update() override;
		void Break();
		void CreateFire();

		std::unique_ptr<Component> Clone() const override;

	};

}