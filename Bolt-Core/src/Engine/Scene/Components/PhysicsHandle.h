#pragma once
#include "Component.h"

namespace Bolt
{

	class BLT_API PhysicsHandle : public Component
	{
	public:
		id_t PhysEngineId;

	public:
		PhysicsHandle();
		~PhysicsHandle();

		void Start() override;

		std::unique_ptr<Component> Clone() const override;

	};

}