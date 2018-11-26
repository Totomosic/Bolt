#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace Aimbooster
{

	const id_t TARGET_HIT_EVENT = EventManager::USER_EVENT_ID + 0;
	const id_t TARGET_FAILED_EVENT = EventManager::USER_EVENT_ID + 1;

	class Target : public Component
	{
	public:
		float Lifetime;
		float StartingSize;
		float MaxSize;
		float CurrentTime;
		float CurrentSize;
		ObjectFactory* Factory;

	public:
		Target(float lifetime, float maxSize, ObjectFactory* factory);

		void Start() override;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}