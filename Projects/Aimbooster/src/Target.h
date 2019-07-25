#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace Aimbooster
{

	class Target : public Component
	{
	public:
		float Lifetime;
		float StartingSize;
		float MaxSize;
		float CurrentTime;
		float CurrentSize;
		ObjectFactory* Factory;
		bool Paused;

	public:
		Target(float lifetime, float maxSize, ObjectFactory* factory);

		void Start() override;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}