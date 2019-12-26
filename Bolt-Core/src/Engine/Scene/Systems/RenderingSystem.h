#pragma once
#include "System.h"

namespace Bolt
{

	class BLT_API RenderingSystem : public System<RenderingSystem>
	{
	public:
		virtual void Update(EntityManager& manager, TimeDelta delta) override;
	};

}