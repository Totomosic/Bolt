#pragma once
#include "System.h"

namespace Bolt
{

	class BLT_API UpdateSystem : public System<UpdateSystem>
	{
	public:
		void Update(EntityManager& entities, TimeDelta dt) override;
	};

}